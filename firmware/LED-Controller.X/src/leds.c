/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leds.c                                    //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: WS2812B LED functions              //
/////////////////////////////////////////////////////

#include <xc.h>
#include "leds.h"


void initLEDs(void) {
    SPI1CON0bits.EN = 0;
    SPI1CON0bits.MST = 1;
    SPI1CON0bits.BMODE = 1;
    SPI1TWIDTH = 3;
    SPI1CON1bits.FST = 1;
    SPI1CON2bits.TXR = 1;
    SPI1BAUD = 12;
    SPI1CON0bits.EN = 1;
    CLC1CONbits.EN = 0;
    CLC1CONbits.MODE = 0b010;  //4 input AND
    CLC1SEL0 = 0b101011;  //SDO1
    CLC1GLS0 = 0x02;
    CLC1GLS1 = 0x00;
    CLC1GLS2 = 0x00;
    CLC1GLS3 = 0x00;
    CLC1POL = 0b00001110;
    CLC1CONbits.EN = 1;
}

void transmitByte(uint8_t b) {
    for(char i = 0; i < 8; ++i) {
        while (!SPI1TXIF);
        if (b & 0b10000000) {
           SPI1TXB = 0b11000000; 
        } else {
           SPI1TXB = 0b10000000; 
        }
        b <<= 1;
    }
}
