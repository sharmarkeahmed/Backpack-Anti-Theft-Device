/*
 * File:   AccelerometerTest.c
 * Author: Sharmarke Ahmed
 *
 * Created on November 24, 2023, 3:31 PM
 */


#include "xc.h"
#include "Accelerometer.h"
#include "PushButton.h"
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
volatile int alarmOn = 0;

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



void loop();
void delay_ms(unsigned int ms);

int main(void) {
    initAlarm();
    initNeopixel();
    initAccelerometer();
    initPushButton();

    loop();
    return 0;
}

void loop() {
    while(1) {
        if(isButtonPressed()) {
            blinkGreen();
            while(!isButtonPressed()) {
                x_accel = getXAcceleration();
                y_accel = getYAcceleration();
                z_accel = getZAcceleration();
                if(z_accel > 30000 && !alarmOn) {
                    turnOnAlarm();
                    alarmOn = 1;
                }
            }
            turnOffAlarm();
            alarmOn = 0;
            blinkRed();
        }
    }
}
