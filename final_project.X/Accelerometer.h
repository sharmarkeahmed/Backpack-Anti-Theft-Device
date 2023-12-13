/*
 * File:   Accelerometer.h
 * Author: Sharmarke Ahmed
 * The Accelerometer library contains an assortment of functions to communicate
 * and gain acceleration values from the LIS3DH accelerometer using a
 * PIC24FJ64GA002 accelerometer. The library uses the I2C1 module of the
 * microcontroller. Ensure that this module is not being used elsewhere.
 * To use this library, connect the SDA1/SCL1 pins of the microcontroller to the
 * SDA/SCL pins of the LIS3DH. Connect the SDO pin of the LIS3DH to ground, and
 * the CS pin of the LIS3DH to Vdd. Connect both pins to a 10kΩ pull up resistor.
 * Initialize the accelerometer with the initAccelerometer() function before 
 * using other functions.
 *
 * Created on November 24, 2023, 2:00 AM
 */

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

// Function declarations
    
/**
 * Initializes the accelerometer by initializing the I2C1 module of the
 * microcontroller, and sending commands to initialize the LIS3DH.
 */
void initAccelerometer();

/**
 * @param address the register in the LIS3DH to read. Refer to Section 7 -
 * Register Mapping (p. 31) of the LIS3DH datasheet manual for specific register
 * addresses.
 * @return 8-bit value corresponding to the value read from the input address
 * register of the LIS3DH
 */
int accel_read(uint8_t address);

/**
 * @param address the register in the LIS3DH to write. Refer to Section 7 -
 * Register Mapping (p. 31) of the LIS3DH datasheet manual for specific register
 * addresses.
 * @param data 8-bit value to write in the specified register address
 */
void accel_write(uint8_t address, uint8_t data);

/**
 * @return x-axis acceleration measured by the sensor
 */
int getXAcceleration();

/**
 * @return  y-axis acceleration measured by the sensor
 */
int getYAcceleration();

/**
 * @return z-axis acceleration measured by the sensorl
 */
int getZAcceleration();

/**
 * The function will detect movement by seeing if the x, y, or z-accelerations 
 * goes above a certain threshold.
 * @return 1 if the accelerometer detected movement, otherwise return 0
 */
int movementDetected();

#ifdef	__cplusplus
}
#endif

#endif	/* ACCELEROMETER_H */
