/* 
 * File:   Neopixel.h
 * Author: Sharmarke Ahmed
 *
 * Created on November 22, 2023, 11:26 AM
 */

#ifndef NEOPIXEL_H
#define	NEOPIXEL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void initNeopixel();
void initTimer1();
void writeColor(int r, int g, int b);
void delay(int delay_in_ms);
uint32_t Wheel(unsigned char WheelPos);
uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu);
unsigned char getR(uint32_t RGBval);
unsigned char getG(uint32_t RGBval);
unsigned char getB(uint32_t RGBval);
void writePacCol(uint32_t PackedColor);
void blinkGreen();
void blinkRed();


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_H */

