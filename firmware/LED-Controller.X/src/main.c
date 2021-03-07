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
#include "leddata.h"

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
        ledToggle();
        setLEDs(1, str1A, 4);
        setLEDs(2, str1A, 4);
        setLEDs(3, str1A, 4);
        setLEDs(4, str1A, 4);
        setLEDs(5, str1A, 4);
        setLEDs(6, str1A, 4);
        __delay_ms(1000);
    }
    
    return;
}
