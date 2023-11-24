/*
 * File:   Neopixel.c
 * Author: Sharmarke Ahmed
 * The Neopixel library contains an assortment of functions useful for 
 * controlling the neopixel, a LED that can change color via serial 
 * communication, on the PIC24FJ64GA002. The Neopixel library uses Timer1 module
 * on the microcontroller. Ensure this module is not being used elsewhere.
 * Created on September 28, 2023, 9:46 PM
 */

#include "xc.h"
#include "Neopixel_asmLib.h"
#include "stdio.h"

#define PERIOD 5 // directive constant for number of milliseconds to delay the
// color writing

// Function declarations:
void initNeopixel();
void writeColor(int r, int g, int b);
void initTimer1();
void delay(int delay_in_ms);
uint32_t Wheel(unsigned char WheelPos);
uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu);
unsigned char getR(uint32_t RGBval);
unsigned char getG(uint32_t RGBval);
unsigned char getB(uint32_t RGBval);
void writePacCol(uint32_t PackedColor);
void blinkGreen();
void blinkRed();

volatile int overflowTMR1 = 0; // count number of times TMR1 overflows

// blinkGreen or blinkRed modes
volatile int modeGreen = 0;
volatile int modeRed = 0;

/**
 * Initializes pin RB6 to be used with the Neopixel on PIC24
 */
void initNeopixel() {
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    TRISBbits.TRISB6 = 0; // Set pin RB6 to output
    LATBbits.LATB6 = 0; // set pin RB6 low
}

/**
 * Helper function to initialize Timer1 on the PIC24 for a 0.2 second period. To
 * be used with the blinkGreen() and blinkRed() functions
 */
void initTimer1() {
    overflowTMR1 = 0;
    T1CONbits.TCKPS = 0b10; // 1:64 prescale
    PR1 = 49999; // 0.2 second period
    // (0.2 seconds) * (16 * 10 ^6)/64 cycles/sec -1 = 49999
    IEC0bits.T1IE = 1; // Enable Timer1 interrupts
    IFS0bits.T1IF = 0; // Reset Timer1 interrupt flag
    T1CONbits.TON = 1; // Turn on TMR1
}

/**
 * Function will write a color with the specified RGB values onto the NeoPixel.
 * Uses RA0 pin.
 * @param r Red color (0-255)
 * @param g Green color (0-255)
 * @param b Blue color (0-255)
 */
void writeColor(int r, int g, int b) {
    
    // bit shifting to make rgb one 24-bit value with rgb
    uint32_t rgb = 0;
    rgb += r;
    rgb = ( rgb << 8);
    rgb += g;
    rgb = (rgb << 8);
    rgb += b;
    
    uint32_t grabBit = 0b100000000000000000000000; // 24 bits
    
    while(grabBit > 0) {
        uint32_t selector = grabBit & rgb;
        
        if(selector != 0) {
            write_1();
        }
        else {
            write_0();
        }
        
        grabBit = grabBit >> 1; // move one bit at a time each time during while loop
    }
    
    delay(5);
    
}

/**
 * @param delay_in_ms time to delay
 */
void delay(int delay_in_ms) {
    for(int i = 0; i < delay_in_ms; i++) {
        wait_1ms();
    }
}

/**
 * Given a character value between 0 to 255, a color value will be returned as a 32 bit
 * integer. Depending on the value of WheelPos, either red and green, green and
 * blue, or red and blue will be turned on at an intensity dependent on the
 * WheelPos value.
 * @param WheelPos integer between 0 to 255
 * @return 32 bit value representing 24 bit RGB colors
 */
uint32_t Wheel(unsigned char WheelPos) {
    WheelPos = 255 - WheelPos;
    
    if(WheelPos < 85){
        return packColor((unsigned char) 255 - WheelPos * 3, 0, (unsigned char) WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return packColor(0, WheelPos * 3, 255- WheelPos * 3);
    }
    
    WheelPos -= 170;
    
    return packColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/**
 * Packs RGB values into a 32-bit RGB value. Note that RGB values are 24 bits
 * total, so the last 8 bits are empty.
 * @param Red 8 bit red value
 * @param Grn 8 bit green value
 * @param Blu 8 bit blue value
 * @return 24-bit RGB value stored in 32-bit unsigned integer
 */
uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu) {
    uint32_t RGBval = ( ((long) Red ) << 16) | ( ( (long) Grn ) << 8) | ( (long) Blu);
    return RGBval;
}

/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit red value as a character
 */
unsigned char getR(uint32_t RGBval) {
    return ( (unsigned char) (RGBval >> 16) );
}

/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit green value as a character
 */
unsigned char getG(uint32_t RGBval) {
    return ( (unsigned char) (RGBval >> 8) );
}


/**
 * Given a 32-bit integer storing 24-bit RGB value, the function will return the
 * 8 bit blue value as a character
 */
unsigned char getB(uint32_t RGBval) {
    return ( (unsigned char) RGBval);
}

/**
 * Given a 32 bit integer containing a 24 bit RGB value, the function will write
 * the NeoPixel connected to port RA0 with the given RGB values.
 */
void writePacCol(uint32_t PackedColor) {
    
    uint32_t grabBit = 0b100000000000000000000000; // 24 bits
    
    while(grabBit > 0) {
        uint32_t selector = grabBit & PackedColor;
        
        if(selector != 0) {
            write_1();
        }
        else {
            write_0();
        }
        
        grabBit = grabBit >> 1; // move one bit at a time each time during while loop
    }
    
    delay(5);
}

/**
 * This function will blink the neopixel green four times in 1.6 seconds.
 */
void blinkGreen() {
    writeColor(0, 255, 0);
    modeRed = 0;
    modeGreen = 1;
    initTimer1();
}

/**
 * This function will blink the neopixel red four times in 1.6 seconds.
 */
void blinkRed() {
    writeColor(255, 0, 0);
    modeGreen = 0;
    modeRed = 1;
    initTimer1();
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt() {
    overflowTMR1++;
    if(overflowTMR1 >= 7) { // Stop blinking & turn off TMR1
        writeColor(0, 0, 0);
        T1CONbits.TON = 0; // Turn off TMR1
    }
    else if(overflowTMR1 % 2) { // overflow is odd, turn off neopixel
        writeColor(0, 0, 0);
    }
    else { // overflow is even, turn on neopixel again
        if(modeGreen) {
            writeColor(0, 255, 0);
        }
        if(modeRed) {
            writeColor(255, 0, 0);
        }
    }
    IFS0bits.T1IF = 0; // Reset Timer1 interrupt flag
}