/* 
 * File:   Accelerometer.h
 * Author: Sharmarke Ahmed
 *
 * Created on November 24, 2023, 3:31 PM
 */

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

// Function declarations
void initAccelerometer();
int accel_read(uint8_t address);
void accel_write(uint8_t address, uint8_t data);
int getXAcceleration();
int getYAcceleration();
int getZAcceleration();
int movementDetected();

#ifdef	__cplusplus
}
#endif

#endif	/* ACCELEROMETER_H */