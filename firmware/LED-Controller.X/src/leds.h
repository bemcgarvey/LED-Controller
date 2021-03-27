/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leds.h                                    //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: WS2812B LED functions              //
/////////////////////////////////////////////////////

#ifndef LEDS_H
#define	LEDS_H

#include <stdint.h>

void initLEDs(void);
void setLEDs(uint8_t output, uint8_t *rgb, uint8_t ledCount);
void clearLEDs(uint8_t output, uint8_t ledCount);

#endif	/* LEDS_H */

