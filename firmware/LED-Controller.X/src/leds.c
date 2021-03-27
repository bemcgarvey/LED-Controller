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
    //Setup CLC1
    CLCSELECT = 0;
    CLCnCONbits.EN = 0;
    CLCnCONbits.MODE = 0b010; //4 input AND
    CLCnSEL0 = 0b101011; //SDO1
    CLCnGLS0 = 0x00;
    CLCnGLS1 = 0x00;
    CLCnGLS2 = 0x00;
    CLCnGLS3 = 0x00;
    CLCnPOL = 0b00001110;
    CLCnCONbits.EN = 1;
    //Setup CLC2
    CLCSELECT = 1;
    CLCnCONbits.EN = 0;
    CLCnCONbits.MODE = 0b010; //4 input AND
    CLCnSEL0 = 0b101011; //SDO1
    CLCnGLS0 = 0x00;
    CLCnGLS1 = 0x00;
    CLCnGLS2 = 0x00;
    CLCnGLS3 = 0x00;
    CLCnPOL = 0b00001110;
    CLCnCONbits.EN = 1;
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

void setLEDs(uint8_t output, uint8_t *rgb, uint8_t ledCount) {
    switch (output) {
        case 0:
            CLCSELECT = 0;
            CLCnGLS0 = 0x02;
            break;
        case 1:
            CLCSELECT = 1;
            CLCnGLS0 = 0x02;
            break;
        case 2: RB3PPS = 0x32; //SPI SDO
            break;
        case 3: RB2PPS = 0x32; //SPI SDO
            break;
        case 4: RC6PPS = 0x32; //SPI SDO
            break;
        case 5: RC5PPS = 0x32; //SPI SDO
            break;
        default: return;
    }
    int count = 3 * ledCount;
    while (count > 0) {
        transmitByte(*rgb);
        ++rgb;
        --count;
    }
    switch (output) {
        case 0:
            CLCSELECT = 0;
            CLCnGLS0 = 0x00;
            break;
        case 1: 
            CLCSELECT = 1;
            CLCnGLS0 = 0x00;
            break;
        case 2: RB3PPS = 0;
            break;
        case 3: RB2PPS = 0;
            break;
        case 4: RC6PPS = 0;
            break;
        case 5: RC5PPS = 0;
            break;
    }
}

void clearLEDs(uint8_t output, uint8_t ledCount) {
    switch (output) {
        case 0:
            CLCSELECT = 0; 
            CLCnGLS0 = 0x02;
            break;
        case 1:
            CLCSELECT = 1;
            CLCnGLS0 = 0x02;
            break;
        case 2: RB3PPS = 0x32; //SPI SDO
            break;
        case 3: RB2PPS = 0x32; //SPI SDO
            break;
        case 4: RC6PPS = 0x32; //SPI SDO
            break;
        case 5: RC5PPS = 0x32; //SPI SDO
            break;
        default: return;
    }
    int count = 3 * ledCount;
    while (count > 0) {
        transmitByte(0x00);
        --count;
    }
    switch (output) {
        case 0:
            CLCSELECT = 0;
            CLCnGLS0 = 0x00;
            break;
        case 1:
            CLCSELECT = 1;
            CLCnGLS0 = 0x00;
            break;
        case 2: RB3PPS = 0;
            break;
        case 3: RB2PPS = 0;
            break;
        case 4: RC6PPS = 0;
            break;
        case 5: RC5PPS = 0;
            break;
    }
}