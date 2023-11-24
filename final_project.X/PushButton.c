/*
 * File:   PushButton.c
 * Author: Sharmarke Ahmed
 * The PushButton library can be used to detect a button press on a button 
 * connected to pin RP15 on the PIC24FJ64GA002. Pin RP15 should be connected
 * to a switch which completes a circuit to ground when pressed. An internal
 * pull up resistor is connected to pin RP15 when initialized. The button should
 * be connected to ground and complete the circuit when pressed. Although
 * debouncing software is provided by the code, it is recommended that a RC
 * low filter circuit is used in connecting to pin RP15 to minimize the effect
 * of noise on button pressed and avoid button presses being registered twice.
 * The library uses the input capture 1 register and Timer2. Ensure these
 * modules are not being used elsewhere.
 * Created on November 22, 2023, 7:18 PM
 */


#include "xc.h"
#include "stdint.h"
#include "Neopixel.h"

void initPushButton();
int isButtonPressed();
volatile uint32_t overflowTMR2 = 0; // 32 bit type to ensure that overflow is large enough
// to count timer overflows even if the PIC24 is on for a very long time
volatile uint32_t currentEventTime = 0;
volatile uint32_t lastEventTime = 0;

volatile int buttonPress = 0;

/**
 * Initialize pin RP15 in detecting button presses. The function will initialize
 * Timer2 to be used with the Input Capture 1 register, and enable a pull up
 * resistor on pin RP15.
 */
void initPushButton() {
    buttonPress = 0;
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFGbits.PCFG9 = 1; // Configure pin RP15 (AN9) as digital
    TRISBbits.TRISB15 = 1; // Set pin RP15 as input
    
    CNPU1bits.CN11PUE = 1; // Turn on pull up resistor for pin RP15 (note that
    // RP15 corresponds to CN11)
    
    overflowTMR2 = 0; // reset overflows for TMR2
    
    /** Initialize Timer2 to have a period of 1 second*/
    T2CON = 0; // initially ensure that T2CON register and TMR2 is zero
    TMR2 = 0;
    T2CONbits.TCKPS = 0b11; // 1:256 prescale
    PR2 = 65535; // maximum period = 2^16 - 1, equivalent to a 
    // 1.04856 second period
    
    /**Configure Timer2 Interrupts*/
    IEC0bits.T2IE = 1; // Enable TMR2 Interrupt
    IFS0bits.T2IF = 0; // Disable TMR2 Interrupt flag
    
    /** Set up Input Capture **/
    IC1CON = 0; // Turn off/reset IC1 register
    IC1CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC1CONbits.ICM = 0b010; // Capture every falling edge
    
    /** Configure pin RB8 for IC1*/
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock PPS
    RPINR7bits.IC1R = 15; // Configure pin RP15 to IC1
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    /** Configure Input Capture 1 Interrupts*/
    IEC0bits.IC1IE = 1; // Enable IC1 interrupt
    IFS0bits.IC1IF = 0; // Reset interrupt flag
    
    T2CONbits.TON = 1; // Turn on TMR2.
    
}

/**
 * @return 1 if a button was pressed, otherwise return 0. Note that after
 * returning that a button was pressed, the function will discard that button
 * press and return 0 until the next press
 */
int isButtonPressed() {
    if(buttonPress) {
        buttonPress = 0;
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * The input capture 1 interrupt ISR will occur each time a falling
 * edge of pin RB15 is detected.
 */
void __attribute__((__interrupt__, __auto_psv__)) _IC1Interrupt() {
    currentEventTime = IC1BUF + overflowTMR2 * 65535;
    if( ( currentEventTime - lastEventTime ) > 124) { // 2ms debounce duration
        
        // Button was pressed
        lastEventTime = currentEventTime;
        buttonPress = 1;
    }
    _IC1IF = 0; // reset IC1 interrupt flag to zero
    return;
}

/**
 * ISR occurs during each rollover of Timer2, and will increment the overflow
 * variable
 */
void __attribute__ ((__interrupt__, __auto_psv__)) _T2Interrupt() {
    overflowTMR2++;
    IFS0bits.T2IF = 0; // Disable TMR2 Interrupt flag
    return;
}