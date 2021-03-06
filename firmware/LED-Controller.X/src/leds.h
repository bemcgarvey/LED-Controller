/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leds.h                                    //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: WS2812B LED functions              //
/////////////////////////////////////////////////////

#ifndef LEDS_H
#define	LEDS_H

#include <stdint.h>

void initLEDs(void);
void transmitByte(uint8_t b);

#endif	/* LEDS_H */

