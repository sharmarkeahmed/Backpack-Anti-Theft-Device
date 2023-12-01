    #include "xc.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define BUFSIZE 10
#define NUMSAMPLES 128
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

int isBackPackOpen(){
    adcval = ((float) 3.3 /1024)*getAvg(); //dark = 3.29, partially open = 1.743 w/ 3.3 V source
    if(adcval == -1){ //buffer is not full in progress
    if(adcval <= 2.5){ //open backpack
        return 1;
    }
        return 0;
        }
int main(){
    adc_init();
    initBuffer();
    
    while(1){
        isBackPackOpen();
    }
    int y = 1;
    }
   
    
    
}
