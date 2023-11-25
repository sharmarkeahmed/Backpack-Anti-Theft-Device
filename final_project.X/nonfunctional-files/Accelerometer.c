/*
 * File:   Accelerometer.c
 * Author: Sharmarke Ahmed
 *
 * Created on November 24, 2023, 2:00 AM
 */


#include "xc.h"
#include "stdint.h"

// Function declarations
void initAccelerometer();
uint8_t getXAcceleration();
uint8_t SPIRead(uint8_t data);

void initAccelerometer() {
    CLKDIVbits.RCDIV = 0; // 16MHz instruction clock
    // Note: pins RP7, RP6, RP5, and RP4 are not analog pins on PIC24
    
    TRISBbits.TRISB7 = 1; // Set RP7 (SDO) as input
    TRISBbits.TRISB6 = 0; // Set RP6 (CS) as output
    TRISBbits.TRISB4 = 0; // Set RP4 (SCL) as output
    TRISBbits.TRISB5 = 0; // Set RP5 (SDA) as output
    
    // Configure pins RP7, RP6, RP4, and RP5 for SPI1CON1 via PPS
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 9; // RP6 mapped to SPI1 slave select output
    RPOR2bits.RP4R = 8; // RP4 mapped to SPI1 clock output
    RPOR2bits.RP5R = 7; // RP5 mapped to SPI1 data output
    RPINR20bits.SDI1R = 7; // RP7 mapped to SPI1 data input
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    /** Configure SPI1CON1 register */
    SPI1CON1 = 0;
    SPI1STAT = 0;

    SPI1CON1bits.MSTEN = 1; // master mode
    SPI1CON1bits.SSEN = 1; // SS1 pin used for slave select enable
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 1;
    
    // Configure SPI1 for a clock frequency of 8 MHz (note: max clock frequency
    // allowed to be used with LIS3DH is 10MHz)
    SPI1CON1bits.PPRE = 0b11; // Primary prescale 1:1
    SPI1CON1bits.SPRE = 0b00; // Secondary prescale 2:1
    SPI1STATbits.SISEL = 0b101;
    SPI1CON2bits.SPIBEN = 1; // Turn on enhanced buffer mode
    SPI1STATbits.SPIEN = 1; // Enable SPI 1 module
    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 1;
}

uint8_t getXAcceleration() {
    return SPIRead(0x28);
}

uint8_t SPIRead(uint8_t data) {
    uint8_t transmitData = data | 0b10000000; // READ (MSB bit)  is 1
    SPI1BUF = transmitData; // transmit data
    while(!IFS0bits.SPI1IF); // wait for data to be received
    SPI1BUF = 0x00;
    IFS0bits.SPI1IF = 0;
    while(!IFS0bits.SPI1IF);
    uint8_t dataReceived = SPI1BUF;
    return dataReceived;
}


