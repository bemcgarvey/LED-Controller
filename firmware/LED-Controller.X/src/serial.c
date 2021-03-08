/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: serial.c                                  //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: serial communication functions     //
/////////////////////////////////////////////////////

#include <xc.h>
#include <stdint.h>
#include "serial.h"
#include "system.h"

void initSerial(void) {
    U1CON1bits.ON = 0;
    U1CON0bits.BRGS = 1;
    U1CON0bits.MODE = 0b0000;
    U1BRGH = 138 >> 8;
    U1BRGL = (uint8_t) 138; //115200
    PIE3bits.U1RXIE = 1;
    IPR3bits.U1RXIP = 0;
    U1CON1bits.ON = 1;
    U1CON0bits.TXEN = 1;
    U1CON0bits.RXEN = 1;
}

void txBytes(void *buf, int nBytes) {
    uint8_t *p = (uint8_t *) buf;
    while (nBytes > 0) {
        while (PIR3bits.U1TXIF == 0);
        U1TXB = *p;
        ++p;
        --nBytes;
    }
}

void __interrupt(irq(U1RX), base(8)) U1_RX_ISR() {
    uint8_t rx;
    while (PIR3bits.U1RXIF) {
        rx = U1RXB;
        switch (rx) {
            case 'f': ledOff();
                break;
            case 'o': ledOn();
                break;
            case 't': ledToggle();
            char buff[] = "ACK";
            txBytes(buff, 3);
                break;
        }
    }
}