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
    CNEN1bits.CN11IE = 1; // Enable change notification interrupt for pin RP15
    IEC1bits.CNIE = 1; // Interrupt request enable for change notification
    IFS1bits.CNIF = 0; // Reset interrupt flag for change notificiation
    
    overflowTMR2 = 0; // reset overflows for TMR2
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

void __attribute__((__interrupt__,__auto_psv__)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
    if(PORTBbits.RB15 == 0) {
        buttonPress = 1;
    }
}