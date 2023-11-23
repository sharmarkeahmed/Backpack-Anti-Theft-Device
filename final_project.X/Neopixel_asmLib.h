/* 
 * File:   Neopixel_asmLib.h
 * Author: Sharmarke Ahmed
 *
 * Created on November 22, 2023, 11:28 AM
 */

#ifndef NEOPIXEL_ASMLIB_H
#define	NEOPIXEL_ASMLIB_H

#ifdef	__cplusplus
extern "C" {
#endif

void write_0(void);
void write_1(void);
void wait_100us(void);
void wait_1ms(void);

#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_ASMLIB_H */
