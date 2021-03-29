/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: system.h                                  //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: system configuration functions     //
/////////////////////////////////////////////////////

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>
#define _XTAL_FREQ  64000000

void initPins(void);
void initOscillator(void);
void initPMD(void);
void initInterrupts(void);

#define ledOn() (LATCbits.LATC1 = 1)
#define ledOff() (LATCbits.LATC1 = 0)
#define ledToggle() (LATCbits.LATC1 ^= 1)

#endif