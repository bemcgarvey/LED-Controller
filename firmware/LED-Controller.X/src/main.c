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
#include "serial.h"

char str[] = "hello world\r\n";
uint8_t g = 0;
uint8_t r = 0;
uint8_t b = 0;

void main(void) {
    initOscillator();
    initPins();
    initPMD();
    initInterrupts();
    initLEDs();
    initSerial();
    INTCON0bits.GIEH = 1;
    while (1) {
        transmitByte(g);
        transmitByte(r);
        transmitByte(b);
        b += 5;
        if (b > 255) {
            b = 0;
        } 
        r += 20;
        if (r > 255) {
            r = 0;
        }
        txBytes(str, 13);
        __delay_ms(200);
    }
    
    return;
}
