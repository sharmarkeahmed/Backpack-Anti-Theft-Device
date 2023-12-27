/*
 * File:   AlarmTest.c
 * Author: Sharmarke Ahmed
 *
 * Created on November 23, 2023, 4:57 PM
 */


#include "xc.h"
#include "Alarm.h"
#include "PushButton.h"
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

#include "stdint.h"

volatile uint32_t overflowTMR4 = 0;
volatile int alarmOn = 0;

void setup();
void loop();
void initTMR4();
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt();

int main(void) {
    setup();
    loop();
}

void setup() {
    initAlarm(10);
    initPushButton();
    initNeopixel();
}

void loop() {
    while(1) {
        if(isButtonPressed()) {
            blinkGreen(); // Indicate device is turned on
//            initTMR4();
            turnOnAlarm();
            while(!isButtonPressed()) {
//                if( ( overflowTMR4 >= 6 ) && !alarmOn) {
//                    alarmOn = 1;
//                    turnOnAlarm();
//                }
//                // Steps for determining if backpack is stolen...
            }
            turnOffAlarm();
//            alarmOn = 0;
            blinkRed(); // Indicate device is turned off
        }
    }
}

void initTMR4() {
    overflowTMR4 = 0;
    T4CON = 0;
    TMR4 = 0;
    T4CONbits.TCKPS = 0b11;
    PR4 = 65535; // period of 1.04856s
    T4CONbits.TON = 1; // TUrn on TMR4
    _T4IE = 1;
    _T4IF = 0;
    
}

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt() {
    overflowTMR4++;
    _T4IF = 0;
}
