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

void main(void) {
    initOscillator();
    initPins();
    initPMD();
    initInterrupts();
    
    while (1) {
        ledToggle();
        __delay_ms(500);
    }
    
    return;
}
