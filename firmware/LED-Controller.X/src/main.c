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
    initControllerMemory();
    initInterrupts();
    initLEDs();
    initSerial();
    initCapture();
    initSystemTimer();
    unsigned int currentTime = 0;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    ledOn();  //TODO add some sort of error status - flash led if invalid ROM?
    while (1) {
        if (currentTime != systemTime) {
            currentTime = systemTime;
            if (currentTime % 10 == 0) {
                
                //ledToggle();
            }
        }
    }

    return;
}
