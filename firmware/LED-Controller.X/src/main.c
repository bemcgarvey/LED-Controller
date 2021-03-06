/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: main.c                                    //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: main program                       //
/////////////////////////////////////////////////////

#include <xc.h>
#include "system.h"
#include "leds.h"

void main(void) {
    initOscillator();
    initPins();
    initPMD();
    initInterrupts();
    initLEDs();
    while (1) {
        ledToggle();
        //LATBbits.LATB3 ^= 1;
        transmitByte(0x00);
        transmitByte(0x00);
        transmitByte(0xff);
        __delay_ms(500);
    }
    
    return;
}
