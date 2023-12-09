/* 
 * File:   LightSensor.h
 * Author: Ryan Fowler, Sharmarke Ahmed
 *
 * Created on December 8, 2023, 1:55 PM
 */

#ifndef LIGHTSENSOR_H
#define	LIGHTSENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

void initLightSensor();
/**
 * checks if light detected is above the voltage threshold needed to set off alarm (2.5 V)
 */
int lightDetected();

#ifdef	__cplusplus
}
#endif

#endif	/* LIGHTSENSOR_H */
