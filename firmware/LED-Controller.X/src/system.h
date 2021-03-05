/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: system.h                                  //
// Target: PIC18F27Q43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: system configuration functions     //
/////////////////////////////////////////////////////

#define _XTAL_FREQ  64000000

void initPins(void);
void initOscillator(void);
void initPMD(void);
void initInterrupts(void);

inline void ledOn(void);
inline void ledOff(void);
inline void ledToggle(void);
