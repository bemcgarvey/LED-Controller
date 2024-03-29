/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leds.c                                    //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: WS2812B LED functions              //
/////////////////////////////////////////////////////

#include <xc.h>
#include "leds.h"
#include "system.h"

void initLEDs(void) {
    //Setup SPI
    SPI1CON0bits.EN = 0;
    SPI1CON0bits.MST = 1;
    SPI1CON0bits.BMODE = 1;
    SPI1TWIDTH = 3;
    SPI1CON1bits.FST = 1;
    SPI1CON2bits.TXR = 1;
    SPI1BAUD = 12;
    SPI1CON0bits.EN = 1;
    //setup CLCs
    for (char i = 0; i < 6; ++i) {
        CLCSELECT = i;
        CLCnCONbits.EN = 0;
        CLCnCONbits.MODE = 0b010; //4 input AND
        CLCnSEL0 = 0b1000000; //SDO1
        CLCnGLS0 = 0x00;
        CLCnGLS1 = 0x00;
        CLCnGLS2 = 0x00;
        CLCnGLS3 = 0x00;
        CLCnPOL = 0b00001110;
        CLCnCONbits.EN = 1;
    }
}

void transmitByte(uint8_t b) {
    for (char i = 0; i < 8; ++i) {
        while (!SPI1TXIF);
        if (b & 0b10000000) {
            SPI1TXB = 0b11000000;
        } else {
            SPI1TXB = 0b10000000;
        }
        b <<= 1;
    }
}

void setLEDs(uint8_t output, uint8_t *rgb, uint8_t ledCount, uint8_t startLED) {
    CLCSELECT = output;
    CLCnGLS0 = 0x02;
    uint8_t *pos = rgb + (3 * startLED);
    while (ledCount > 0) {
        transmitByte(*pos++);
        transmitByte(*pos++);
        transmitByte(*pos++);
        --ledCount;
        if (ledCount == startLED) {
            pos = rgb;
        }
    }
    CLCnGLS0 = 0x00;
}

void clearLEDs(uint8_t output, uint8_t ledCount) {
    CLCSELECT = output;
    CLCnGLS0 = 0x02;
    while (ledCount > 0) {
        transmitByte(0x00);
        transmitByte(0x00);
        transmitByte(0x00);
        --ledCount;
    }
    CLCnGLS0 = 0x00;
}