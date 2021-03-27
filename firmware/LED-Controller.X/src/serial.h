/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: serial.h                                  //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: serial communication functions     //
/////////////////////////////////////////////////////

#ifndef SERIAL_H
#define	SERIAL_H

extern char serialConnected;
extern int8_t doTest;
extern char updatePointers;
extern char updateFromROM;

void initSerial(void);
//void txBytes(void *buf, int nBytes);

#endif	/* SERIAL_H */

