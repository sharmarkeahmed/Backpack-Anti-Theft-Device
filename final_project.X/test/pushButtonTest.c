/*
 * File:   pushButtonTest.c
 * Author: Sharmarke Ahmed
 * This code was used to verify operation of the PushButton library for
 * determining button presses to change the mode of the PIC24
 *
 * Created on November 22, 2023, 8:45 PM
 */

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

#include "xc.h"
#include "PushButton.h"
#include "Neopixel.h"

void setup();
void loop();

int main(void) {
    setup();
    loop();
}

void setup() {
    initPushButton();
    initNeopixel();
}

void loop() {
    while(1) {
        if(isButtonPressed()) {
            blinkGreen(); // Indicate device is turned on
            while(!isButtonPressed()) {
                // Steps for determining if backpack is stolen...
            }
            blinkRed(); // Indicate device is turned off
        }
    }
}
