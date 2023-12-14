#include "xc.h"
#include "stdint.h"
#include "Accelerometer.h"
#include "PushButton.h"
#include "Alarm.h"
#include "Neopixel.h"
#include "LightSensor.h"


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


// The device can only run for a maximum of ~19 hours with a 32 bit timer. Use
// 64-bit values for very long term applications
volatile uint32_t overflowTMR4 = 0;
volatile uint32_t time1 = 0;
volatile uint32_t time2 = 0;


void setup();
void loop();
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt();



int main(){
    setup();
    loop();
    
    return 0;
}

void setup() {
    initAlarm(10);
    initAccelerometer();
    initNeopixel();
    initPushButton();
    initLightSensor();
    
    // Initialize TMR4 for a 1 second period
    T4CON = 0;
    TMR4 = 0;
    
    T4CONbits.TCKPS = 0b11;
    PR4 = 65535;
    
    // Enable interrupts and disable interrupt flag for TMR4
    _T4IE = 1;
    _T4IF = 0;
    T4CONbits.TON = 1; // Turn on TMR4
}

void loop() {
    int exitMechanism = 0; // variable to track whether or not the user wishes
    // to turn off the anti theft mechanism after the device is turned on
    while(1) {
        if(isButtonPressed()) { // Turn on security mechanism
            blinkGreen(); // indicate mechanism is ON
            time1 = TMR4 + overflowTMR4 * 65535;
            time2 = time1;
            uint32_t difference = time2 - time1;
            while( difference < (uint32_t) (65535 * 7) && !exitMechanism) { // wait 7 seconds for person to store
                // device before actually turning on security mechanism
                time2 = TMR4 + overflowTMR4 * 65535;
                difference = time2 - time1;
                if(isButtonPressed()) { // Turn off device
                    exitMechanism = 1;
                }
            }
            if(!exitMechanism) { // Turn on security mechanism
                while(!exitMechanism) {
                    if(isButtonPressed()) { // Check if user wants to turn off
                        // the device
                        exitMechanism = 1;
                    }
                    if(movementDetected() || lightDetected()) {
                        // wait 4 seconds, make sure the owner of the backpack
                        // is not about to turn off the device first
                        time1 = TMR4 + overflowTMR4 * 65535;
                        time2 = time1;
                        difference = time2 - time1;
                        while( difference < (uint32_t) (65535 * 4) && !exitMechanism) { 
                            // wait 4 seconds, make sure the owner of the 
                            // backpack is not about to turn off the device 
                            // first
                            time2 = TMR4 + overflowTMR4 * 65535;
                            difference = time2 - time1;
                            if(isButtonPressed()) {
                                exitMechanism = 1;
                            }
                        }
                        
                        if(!exitMechanism) { // 4-second waiting period
                            // ended, turn on alarm, backpack is stolen!
                            turnOnAlarm();
                            while(!exitMechanism) {
                                if(isButtonPressed()) { // alarm will
                                    // continue to play until button is pressed
                                    exitMechanism = 1;
                                }
                            }
                        }
                    }
                }
            } // turn off device
            exitMechanism = 0;
            blinkRed(); // indicate mechanism is OFF
            turnOffAlarm();
        }
    }
}

/**
 * Interrupts on TMR4 overflow, incrementing the global variable to keep
 * track of how many times TMR4 has overflowed
 */
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt() {
    overflowTMR4++;
    _T4IF = 0; // Reset TImer4 interrupt flag
}
