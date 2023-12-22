/*
 * File:   Accelerometer.c
 * Author: Sharmarke Ahmed
 * The Accelerometer library contains an assortment of functions to communicate
 * and gain acceleration values from the LIS3DH accelerometer using a
 * PIC24FJ64GA002 accelerometer. The library uses the I2C1 module of the
 * microcontroller. Ensure that this module is not being used elsewhere.
 * To use this library, connect the SDA1/SCL1 pins of the microcontroller to the
 * SDA/SCL pins of the LIS3DH. Connect the SDO pin of the LIS3DH to ground, and
 * the CS pin of the LIS3DH to Vdd. Connect both pins to a 10k? pull up resistor.
 * Initialize the accelerometer with the initAccelerometer() function before 
 * using other functions.
 *
 * Created on November 24, 2023, 2:00 AM
 */


#include "xc.h"
#include "stdint.h"

#define STATUS_REG_AUX 0x07
#define OUT_ADC1_L 0x08
#define OUT_ADC1_H 0x09
#define OUT_ADC2_L 0x0A
#define OUT_ADC2_H 0x0B
#define OUT_ADC3_L 0x0C
#define OUT_ADC3_H 0x0D
#define INT_COUNTER_REG 0x0E
#define WHO_AM_I 0x0F
#define TEMP_CFG_REG 0x1F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define CTRL_REG6 0x25
#define REFERENCE 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_THS 0x32
#define INT1_CFG 0x38
#define DEVICE_ADDRESS 0x30

// Function declarations
void initAccelerometer();
int accel_read(uint8_t address);
void accel_write(uint8_t address, uint8_t data);
int getXAcceleration();
int getYAcceleration();
int getZAcceleration();
int movementDetected();
void delay_ms_accel(unsigned int ms);

/**
 * Initializes the accelerometer by initializing the I2C1 module of the
 * microcontroller, and sending commands to initialize the LIS3DH.
 */
void initAccelerometer() {
    CLKDIVbits.RCDIV = 0; // 16MHz instruction clock
    // Note: SDA1/SCL1 are not analog pins; don't need to set to digital mode
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    /**Initialize I2C*/
    
    // Initially turn off I2C when setting baud rate generator
    I2C1CONbits.I2CEN = 0;
    IFS1bits.MI2C1IF = 0; // Turn off interrupt
    
    I2C1BRG = 157; // Fscl = 100KHz
    I2C1CONbits.I2CEN = 1; // Turn on I2C
    
    // Send commands to initialize the LIS3DH. Refer to P.13 of manual
    delay_ms_accel(100);
    accel_write(CTRL_REG5, 0b10000000); // reboot memory content
    delay_ms_accel(100);
    accel_write(CTRL_REG5, 0x00);
    delay_ms_accel(100);
    accel_write(CTRL_REG1, 0x77);
    accel_write(CTRL_REG2, 0x01);
    accel_write(CTRL_REG3, 0x40);
    accel_write(CTRL_REG4, 0x84);
    accel_write(INT1_THS, 0x20);
    accel_write(INT1_CFG, 0x2A);
}

/**
 * @param address the register in the LIS3DH to read. Refer to Section 7 -
 * Register Mapping (p. 31) of the LIS3DH datasheet manual for specific register
 * addresses.
 * @return 8-bit value corresponding to the value read from the input address
 * register of the LIS3DH
 */
int accel_read(uint8_t address) {
    I2C1CONbits.SEN = 1; // initialize start condition
    while(I2C1CONbits.SEN == 1); // wait for start bit to be sent
    IFS1bits.MI2C1IF = 0; // Clear interrupt flag
    I2C1TRN = DEVICE_ADDRESS; // 8 bits. consists of slave address (0b00110001) and
    // the last bit for writing (0)
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = address; // data register byte
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = DEVICE_ADDRESS | 0x01; // 8 bits. consists of slave address (0b00110001) and
    // the last bit for reading (1)
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    I2C1CONbits.SEN = 1; // initialize start condition
    while(I2C1CONbits.SEN == 1); // wait for start bit to be sent
    IFS1bits.MI2C1IF = 0; // Clear interrupt flag
    I2C1TRN = DEVICE_ADDRESS | 0x01; // 8 bits. consists of slave address (0b00110001) and
    // the last bit for reading (1)
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    I2C1CONbits.RCEN = 1; // Enable receive mode
    while(I2C1STATbits.RBF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.ACKDT = 0; // Send ACK
    int retVal = I2C1RCV;
    I2C1CONbits.PEN = 1; // stop bit
    while(I2C1CONbits.PEN == 1); // wait for stop bit to be sent
    return retVal;
}

/**
 * @param address the register in the LIS3DH to write. Refer to Section 7 -
 * Register Mapping (p. 31) of the LIS3DH datasheet manual for specific register
 * addresses.
 * @param data 8-bit value to write in the specified register address
 */
void accel_write(uint8_t address, uint8_t data) {
    I2C1CONbits.SEN = 1; // initialize start condition
    while(I2C1CONbits.SEN == 1); // wait for start bit to be sent
    IFS1bits.MI2C1IF = 0; // Clear interrupt flag
    I2C1TRN = DEVICE_ADDRESS; // 8 bits. consists of slave address (0b00110001) and
    // the last bit for write (0)
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = address; // data register byte
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = data; // data to be sent
    while(IFS1bits.MI2C1IF == 0); // wait for interrupt flag
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.PEN = 1; // stop bit
    while(I2C1CONbits.PEN == 1); // wait for stop bit to be sent
}

/**
 * @return x-axis acceleration measured by the sensor
 */
int getXAcceleration() {
    int x = accel_read(OUT_X_H) << 8;
    x |= accel_read(OUT_X_L);
    return x;
}

/**
 * @return  y-axis acceleration measured by the sensor
 */
int getYAcceleration() {
    int y = accel_read(OUT_Y_H) << 8;
    y |= accel_read(OUT_Y_L);
    return y;
}

/**
 * @return z-axis acceleration measured by the sensorl
 */
int getZAcceleration() {
    int z = accel_read(OUT_Z_H) << 8;
    z |= accel_read(OUT_Z_L);
    return z;
}

/**
 * The function will detect movement by seeing if the x, y, or z-accelerations 
 * goes above a certain threshold.
 * @return 1 if the accelerometer detected movement, otherwise return 0
 */
int movementDetected() {
    if(getZAcceleration() > 15000) {
        return 1;
    }
    else if(getYAcceleration() > 15000) {
        return 1;
    }
    else if(getZAcceleration() > 15000) {
        return 1;
    }
    return 0;
}

/**
 * @param ms time to delay (in ms)
 */
void delay_ms_accel(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}
