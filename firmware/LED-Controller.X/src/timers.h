/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: timers.h                                  //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: Timer functions                    //
/////////////////////////////////////////////////////

#ifndef TIMERS_H
#define	TIMERS_H

extern unsigned int systemTime;

void initTimer1(void);
void initSystemTimer(void);


#endif	/* TIMERS_H */

