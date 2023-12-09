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

volatile uint64_t overflowTMR4 = 0;
volatile uint64_t time1 = 0;
volatile uint64_t time2 = 0;


void setup();
void loop();



int main(){
    setup();
    loop();
    
    return 0;
}

void setup() {
    initAlarm();
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
    int exit = 0;
    while(1) {
        if(isButtonPressed()) { // Turn on security mechanism
            blinkGreen();
            time1 = TMR4 + overflowTMR4 * 65535;
            time2 = time1;
            uint64_t difference = time2 - time1;
            uint64_t sevenSecondsInCycles = 65535*7;
            while( difference < sevenSecondsInCycles && !exit) { // wait 7 seconds for person to store
                // device before actually turning on security mechanism
                time2 = TMR4 + overflowTMR4 * 65535;
                difference = time2 - time1;
                if(isButtonPressed()) {
                    exit = 1;
//                    blinkRed();
                }
            }
            if(!exit) {
                while(!exit) {
                    if(isButtonPressed()) {
                        exit = 1;
//                        blinkRed();
//                        turnOffAlarm();
                    }
                    if(movementDetected() || lightDetected()) {
                        turnOnAlarm();
                    }
                }
            }
            exit = 0;
            blinkRed();
            turnOffAlarm();
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt() {
    overflowTMR4++;
    _T4IF = 0; // Reset TImer4 interrupt flag
}
