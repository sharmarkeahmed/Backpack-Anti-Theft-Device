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
 *
 * Created on November 23, 2023, 4:59 PM
 */

#ifndef ALARM_H
#define	ALARM_H

#ifdef	__cplusplus
extern "C" {
#endif

// Function declarations
    
/**
 * Initializes pin RP14 as output for the piezoelectric buzzer, as well as 
 * the output compare register and Timer2 used to send pulses to the buzzer.
 */
void initAlarm();

/**
 * Turns on the alarm
 */
void turnOnAlarm();

/**
 * Turns off the alarm
 */
void turnOffAlarm();


#ifdef	__cplusplus
}
#endif

#endif	/* ALARM_H */

