/*
 * File:   PushButton.c
 * Author: Sharmarke Ahmed
 * The PushButton library can be used to detect a button press on a button 
 * connected to pin RP15 on the PIC24FJ64GA002. Pin RP15 should be connected
 * to a switch which completes a circuit to ground when pressed. An internal
 * pull up resistor is connected to pin RP15 when initialized. The button should
 * be connected to ground and complete the circuit when pressed. A low pass
 * filter must be connected to pin RP15 to filter out high frequency noise
 * caused by a switch bounce. This library uses the change notification
 * interrupt on the PIC24.
 * Created on November 22, 2023, 7:18 PM
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
 * the change notification interrupt for the pin and the internal pull-up
 * resistor for it.
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