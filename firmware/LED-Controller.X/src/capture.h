/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: capture.h                                 //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: RC input capture                   //
/////////////////////////////////////////////////////

#ifndef CAPTURE_H
#define	CAPTURE_H

void initCapture(void);
int currentPWMus(void);
int currentPWMusInt(void);

#endif	/* CAPTURE_H */

