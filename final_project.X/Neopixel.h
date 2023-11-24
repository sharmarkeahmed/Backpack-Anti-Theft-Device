/*
 * File:   Neopixel.c
 * Author: Sharmarke Ahmed
 * The Neopixel library contains an assortment of functions useful for 
 * controlling the neopixel, a LED that can change color via serial 
 * communication, on the PIC24FJ64GA002. Connect the neopixel to pin RB13. 
 * The Neopixel library uses Timer1 module on the microcontroller. Ensure this 
 * module is not being used elsewhere.
 * 
 * Created on November 22, 2023, 11:26 AM
 */

#ifndef NEOPIXEL_H
#define	NEOPIXEL_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Initializes pin RB6 to be used with the Neopixel on PIC24
 */
void initNeopixel();

/**
 * Function will write a color with the specified RGB values onto the NeoPixel.
 * Uses RA0 pin.
 * @param r Red color (0-255)
 * @param g Green color (0-255)
 * @param b Blue color (0-255)
 */
void writeColor(int r, int g, int b);

/**
 * Given a character value between 0 to 255, a color value will be returned as a 32 bit
 * integer. Depending on the value of WheelPos, either red and green, green and
 * blue, or red and blue will be turned on at an intensity dependent on the
 * WheelPos value.
 * @param WheelPos integer between 0 to 255
 * @return 32 bit value representing 24 bit RGB colors
 */
uint32_t Wheel(unsigned char WheelPos);

/**
 * Packs RGB values into a 32-bit RGB value. Note that RGB values are 24 bits
 * total, so the last 8 bits are empty.
 * @param Red 8 bit red value
 * @param Grn 8 bit green value
 * @param Blu 8 bit blue value
 * @return 24-bit RGB value stored in 32-bit unsigned integer
 */
uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu);

/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit red value as a character
 */
unsigned char getR(uint32_t RGBval);

/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit green value as a character
 */
unsigned char getG(uint32_t RGBval);

/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit blue value as a character
 */
unsigned char getB(uint32_t RGBval);

/**
 * Given a 32 bit integer containing a 24 bit RGB value, the function will write
 * the NeoPixel connected to port RA0 with the given RGB values.
 */
void writePacCol(uint32_t PackedColor);

/**
 * This function will blink the neopixel green four times in 1.6 seconds.
 */
void blinkGreen();

/**
 * This function will blink the neopixel red four times in 1.6 seconds.
 */
void blinkRed();


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIXEL_H */
