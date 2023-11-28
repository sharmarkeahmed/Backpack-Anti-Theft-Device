/*
 * File:   AccelerometerTest.c
 * Author: Sharmarke Ahmed
 *
 * Created on November 24, 2023, 3:31 PM
 */


#include "xc.h"
#include "Accelerometer.h"
#include "stdint.h"
#include "Alarm.h"
#include "Neopixel.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

volatile int x_accel;
volatile int y_accel;
volatile int z_accel;
volatile int alarmNotOn = 0;

#define WHO_AM_I 0x0F
#define CTRL_REG0 0x1E
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
#define INT1_THS 0x32
#define INT1_CFG 0x38



void loop();
void delay_ms(unsigned int ms);

int main(void) {
    initAlarm();
    initNeopixel();
    initAccelerometer();
    delay_ms(100);
    accel_write(CTRL_REG5, 0b10000000); // reboot memory content
    delay_ms(100);
    accel_write(CTRL_REG5, 0x00);
    delay_ms(100);
    
    // Initialize LIS3DH
    if(accel_read(WHO_AM_I) != 0x33) {
        return 1; // Something is wrong!
    }
//    accel_write(CTRL_REG0, 0b0010000); // p. 34, default values
//    accel_write(CTRL_REG1, 0b01110111); // 400Hz, Normal, ZYX axis enabled
//    accel_write(CTRL_REG4, 0b10111000); // block data not updated until MSB
//    accel_write(CTRL_REG5, 0b01000000); // FIFO enable
    
    accel_write(CTRL_REG1, 0x77);
    accel_write(CTRL_REG2, 0x01);
    accel_write(CTRL_REG3, 0x40);
    accel_write(CTRL_REG4, 0x84);
    accel_write(INT1_THS, 0x20);
    accel_write(INT1_CFG, 0x2A);
    // and LSB reating, 16g scale selection, high resolution output mode
    
//    // Verify correct register values
//    volatile int reg0setting = accel_read(CTRL_REG0); // 0b0010000
//    volatile int reg1setting = accel_read(CTRL_REG1); // 0b01110111
//    volatile int reg2setting = accel_read(CTRL_REG2); // default
//    volatile int reg3setting = accel_read(CTRL_REG3); // default
//    volatile int reg4setting = accel_read(CTRL_REG4); // 0b10111000
//    volatile int y = 1;
    loop();
    return 0;
}

void loop() {
    while(1) {
            x_accel = getXAcceleration();
            y_accel = getYAcceleration();
            z_accel = getZAcceleration();
            if(z_accel > 30000 && !alarmNotOn) {
                turnOnAlarm();
                blinkRed();
                alarmNotOn = 1;
            }
    }
}

/**
 * @param ms the amount of time (in milliseconds) to delay the PIC24 on a 16MHz
 * instruction clock
 */
void delay_ms(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}