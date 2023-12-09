/*
 * File:   LightSensor.c
 * Author: Ryan Fowler
 * The Light Sensor library uses a light sensor that changes its resistance
 * based on how much light it detects. It is used in a voltage divider with
 * a 4.7 k ohm resistor and the voltage is read using a peripheral pin on the
 * microcontroller. This value is analog so an analog to digital converter
 * converts it into a voltage. Compatible with 3.0 V and 3.3 V sources
 *
 * Created on December 1, 2023, 11:35 AM
 */

#ifndef LIGHTSENSOR_H
#define	LIGHTSENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Initializes light sensor pin as well as setting up AD conversion
 */
void initLightSensor();

/**
 * checks if light detected is above the voltage threshold needed to set off alarm (2.5 V)
 */
int lightDetected();

#ifdef	__cplusplus
}
#endif

#endif	/* LIGHTSENSOR_H */
