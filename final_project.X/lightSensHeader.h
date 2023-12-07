/* 
 * File:   lightSensHeader.h
 * Author: fowle314
 *
 * Created on December 7, 2023, 11:53 AM
 */

#ifndef LIGHTSENSHEADER_H
#define	LIGHTSENSHEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
#define BUFSIZE 10
#define NUMSAMPLES 128
#define BATTERYVOLT 3
#define COMPVOLT 3.3
#define COMPTHRESHOLD 2.5
#define BATTERYTHRESHOLD 
volatile int adc_buffer[BUFSIZE];
volatile int buffer_index = 0;
volatile float adcval = 0;

void delay_ms(unsigned long int x){
    while(x-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}

void adc_init(){ // initializes ADC and assigns it to timer 3, no arguments, no return values
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

void initBuffer(){ //initializes buffer to size of 10, no arguments, no return values
    for(int i=0; i<BUFSIZE; i++){
        adc_buffer[i] = 0;
    }
}

void putVal(int ADCvalue){ //Fills buffer with ADC values, the ADC value is the argument, no return values
    adc_buffer[buffer_index++] = ADCvalue;
    if(buffer_index >= BUFSIZE){
        buffer_index = 0;
    }
}

int getAvg(){ // averages the values in the buffer, no arguments, no return values
    unsigned long int sum = 0;
    int average;
    
    for(int i=0; i<BUFSIZE; i++){
        sum = sum + adc_buffer[i];
    }
    average = sum/BUFSIZE;
    return average;
    }


void __attribute__((interrupt,auto_psv))_ADC1Interrupt(){ //everytime buffer is full it puts the value in the buffer
    _AD1IF = 0;
    putVal(ADC1BUF0);
}

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



#ifdef	__cplusplus
}
#endif

#endif	/* LIGHTSENSHEADER_H */
