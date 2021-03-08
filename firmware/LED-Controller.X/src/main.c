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
#include "timers.h"

void main(void) {
    initOscillator();
    initPins();
    initPMD();
    initInterrupts();
    initLEDs();
    initSerial();
    initCapture();
    initSystemTimer();
    unsigned int currentTime = 0;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    ledOn();
    while (1) {
        if (currentTime != systemTime) {
            currentTime = systemTime;
            if (currentTime % 10 == 0) {
                setLEDs(1, str1A, 4);
                setLEDs(2, str1A, 4);
                setLEDs(3, str1A, 4);
                setLEDs(4, str1A, 4);
                setLEDs(5, str1A, 4);
                setLEDs(6, str1A, 4);
                ledToggle();
            }
        }
    }

    return;
}
