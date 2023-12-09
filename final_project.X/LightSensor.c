/*
 * File:   LightSensor.c
 * Author: Ryan Fowler, Sharmarke Ahmed
 * The Light Sensor library uses a light sensor that changes its resistance
 * based on how much light it detects. It is used in a voltage divider with
 * a 4.7 k ohm resistor and the voltage is read using a peripheral pin on the
 * microcontroller. This value is analog so an analog to digital converter
 * converts it into a voltage. Compatible with 3.0 V and 3.3 V sources
 *
 * Created on December 1, 2023, 11:35 AM
 */

#include "xc.h"
#include "stdint.h"

#define BUFSIZE 10
#define NUMSAMPLES 128
#define BATTERYVOLT 3
#define COMPVOLT 3.3
#define COMPTHRESHOLD 2.5
#define BATTERYTHRESHOLD 2.5
volatile int adc_buffer[BUFSIZE];
volatile int buffer_index = 0;
volatile float adcval = 0;

void delay_ms(unsigned long int ms);
void initLightSensor();
void initBuffer();
void putVal(int ADCvalue);
int getAvg();
void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt();
int lightDetected();

/**
 * Assembly function that uses "nop" to delay for the specified
 * number of miliseconds.
 */
void delay_ms(unsigned long int ms){
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

/**
 * Initializes light sensor pin as well as setting up AD conversion
 */

void initLightSensor(){ // initializes ADC and assigns it to timer 3, no arguments, no return values
    _RCDIV = 0;
    TRISAbits.TRISA0 = 1;
    
    AD1PCFGbits.PCFG0 = 0;
    
    AD1CON2bits.VCFG = 0b000;
    AD1CON3bits.ADCS = 1;
    AD1CON1bits.SSRC = 0b010;
    AD1CON3bits.SAMC = 1;
    AD1CON1bits.FORM = 0;
    
    AD1CON1bits.ASAM = 1;
    AD1CON2bits.SMPI = 0;
    AD1CON1bits.ADON = 1;
    
    _AD1IF = 0;
    _AD1IE = 1;
    
    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b10;
    PR3 = 15624;
    T3CONbits.TON = 1;
}

/**
 * creates Buffer array to store ADC buffer values
 */

void initBuffer(){ //initializes buffer to size of 10, no arguments, no return values
    for(int i=0; i<BUFSIZE; i++){
        adc_buffer[i] = 0;
    }
}

/**
 * Takes the value from the buffer and puts it in the array.
 */

void putVal(int ADCvalue){ //Fills buffer with ADC values, the ADC value is the argument, no return values
    adc_buffer[buffer_index++] = ADCvalue;
    if(buffer_index >= BUFSIZE){
        buffer_index = 0;
    }
}

/**
 * averages the values of the array.
 */

int getAvg(){ // averages the values in the buffer, no arguments, no return values
    unsigned long int sum = 0;
    int average;
    
    for(int i=0; i<BUFSIZE; i++){
        sum = sum + adc_buffer[i];
    }
    average = sum/BUFSIZE;
    return average;
    }

/**
 * waits until buffer is full and puts value in array.
 */
void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(){ //everytime buffer is full it puts the value in the buffer
    _AD1IF = 0;
    putVal(ADC1BUF0);
}

/**
 * checks if light detected is above the voltage threshold needed to set off alarm (2.5 V)
 */

int lightDetected(){
    adcval = ((float) 3.0 /1024)*getAvg(); //dark = 3.29, partially open = 1.743 w/ 3.3 V source || dark = 2.997, partially open = 1.863 w/ 3.0 V source
    if(adc_buffer[9] == 0){ //buffer is not full in progress
        return -1;
    }
    else if(adcval <= 2.5){ //open backpack
        return 1;
    }
        return 0;
}
