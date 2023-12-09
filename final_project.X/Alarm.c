/*
 * File:   Alarm.c
 * Author: Sharmarke Ahmed
 * The Alarm library is used to control a piezoelectric buzzer which generates a
 * continuous sound when connected to 3.3V on pin RP14 on the PIC24FJ64GA002.
 * The library uses Timer2 and the output compare 1 register on the 
 * microcontroller. Ensure these modules are not being used elsewhere. When the
 * alarm is turned on, the library will send pulses to the buzzer with a
 * frequency of 0.476 Hz.
 *
 * Created on November 23, 2023, 4:11 PM
 */


#include "xc.h"

// Function declarations
void initAlarm();
void turnOnAlarm();
void turnOffAlarm();

/**
 * Initializes pin RP14 as output for the piezoelectric buzzer, as well as 
 * the output compare register and Timer2 used to send pulses to the buzzer.
 */
void initAlarm() {
    CLKDIVbits.RCDIV = 0; // 16 MHz clock
    AD1PCFGbits.PCFG10 = 1; // Configure pin RP14 (AN10) as digital
    TRISBbits.TRISB14 = 0; // Configure pin RP14 as output
    LATBbits.LATB14 = 0; // Initially have pin RP14 LOW
    
    /** Set up TMR3 to have maximum period **/
    T2CON = 0; // ensure TMR2 is reset
    TMR2 = 0; // ensure TMR2 starts at zero
    T2CONbits.TCKPS = 0b11; // 1:256 prescale
    PR2 = 65535; // period of ~1.05 seconds
    
    /** Map output compare 1 register to pin RP14 via PPS */
    
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock PPS
    RPOR7bits.RP14R = 18; // RP14 configured for output compare via PPS select
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS

}

/**
 * Turns on the alarm
 */
void turnOnAlarm() {
    
    /** Configure Output Compare 1 register*/
    
    OC1CON = 0; // Reset OC1 register initially
    OC1R = 0;
    OC1CONbits.OCM = 0b011; // Toggle mode
    OC1CONbits.OCTSEL = 0; // TMR2 clock source for output compare 
    TMR2 = 0;
    T2CONbits.TON = 1;
}

/**
 * Turns off the alarm
 */
void turnOffAlarm() {
    OC1CON = 0; // Reset OC1 register
    T2CONbits.TON = 0;
    LATBbits.LATB14 = 0;
}
