/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: serial.h                                  //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: serial communication functions     //
/////////////////////////////////////////////////////

#ifndef SERIAL_H
#define	SERIAL_H

void initSerial(void);
void txBytes(void *buf, int nBytes);

#endif	/* SERIAL_H */

