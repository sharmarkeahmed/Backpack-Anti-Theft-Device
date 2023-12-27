/* 
 * File:   Accelerometer.h
 * Author: sharm
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
uint8_t getXAcceleration();
uint8_t SPIRead(uint8_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* ACCELEROMETER_H */

