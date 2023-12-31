/*
 * File:   Alarm.h
 * Author: Sharmarke Ahmed, Ryan Fowler
 * The Alarm library is used to control a piezoelectric buzzer which generates a
 * continuous sound when connected to 3.3V on pin RP14 on the PIC24FJ64GA002.
 * The library uses Timer2 and the output compare 1 register on the 
 * microcontroller. Ensure these modules are not being used elsewhere. When the
 * alarm is turned on, the library will send pulses to the buzzer with an
 * input frequency greater than 0.477 Hz.
 *
 * Created on November 23, 2023, 4:11 PM
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
 * 
 * @param freq the minimum frequency allowed is 0.4768444343 Hz, anything lower
 * than this will overflow the period. The frequency at which that alarm beeps 
 * can be altered by the user
 */
void initAlarm(double freq);

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

