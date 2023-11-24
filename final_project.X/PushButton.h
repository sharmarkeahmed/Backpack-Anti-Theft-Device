/*
 * File:   PushButton.h
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
 * 
 * Created on November 22, 2023, 8:45 PM
 */

#ifndef PUSHBUTTON_H
#define	PUSHBUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Initialize pin RP15 in detecting button presses. The function will initialize
 * Timer2 to be used with the Input Capture 1 register, and enable a pull up
 * resistor on pin RP15.
 */
void initPushButton();

/**
 * @return 1 if a button was pressed, otherwise return 0. Note that after
 * returning that a button was pressed, the function will discard that button
 * press and return 0 until the next press
 */
int isButtonPressed();


#ifdef	__cplusplus
}
#endif

#endif	/* PUSHBUTTON_H */

