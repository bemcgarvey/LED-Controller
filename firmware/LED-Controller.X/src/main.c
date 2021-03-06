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
#include "capture.h"

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
    initCapture();
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    while (1) {
        int pwm = currentPWMus();
        if (pwm < 950) {
            r = 0;
            g = 0;
            b = 0;
        } else if (pwm < 1200) {
            r = 255;
            g = 0;
            b = 0;
        } else if (pwm < 1800) {
            r = 0;
            g = 0;
            b = 255;
        } else if (pwm < 2000) {
            r = 0;
            g = 255;
            b = 0;
        } else {
            r = 255;
            g = 255;
            b = 255;
        }
        transmitByte(g);
        transmitByte(r);
        transmitByte(b);
        txBytes(str, 13);
        __delay_ms(200);
    }
    
    return;
}
