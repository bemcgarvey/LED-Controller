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
    for (uint8_t i = 0; i < 6; ++i) {
        clearLEDs(i, 255);
    }
    if (!serialConnected) {
        
    }
    while (1) {
        if (currentTime != systemTime) {
            currentTime = systemTime;
            if (currentTime % 10 == 0) {
                
                //ledToggle();
            }
        }
        if (serialConnected) {
            if (doTest != -1) {
                clearLEDs((uint8_t)doTest, 255);
                setLEDs((uint8_t)doTest, &controller.bytes[4], controller.bytes[0]);
                doTest = -1;
                copyFromROM();
                calculatePointers();
            }
        } else {
            
        }
    }

    return;
}
