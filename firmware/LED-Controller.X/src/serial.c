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
#include "leddata.h"
#include "version.h"

enum rxState {
    RX_IDLE, WAIT_START1, WAIT_START2, WAIT_COMMAND, WAIT_DATA
};
volatile enum rxState state;
volatile int bytesNeeded;
volatile enum rxState lastCommand;
volatile uint8_t txBuf[4];
volatile uint16_t txBytes;
volatile uint8_t *txSource;

#define txStart() (PIE3bits.U1TXIE = 1)
#define txStop()  (PIE3bits.U1TXIE = 0)

void initSerial(void) {
    U1CON1bits.ON = 0;
    U1CON0bits.BRGS = 1;
    U1CON0bits.MODE = 0b0000;
    U1BRGH = 138 >> 8;
    U1BRGL = (uint8_t) 138; //115200
    PIE3bits.U1RXIE = 1;
    IPR3bits.U1RXIP = 0;
    IPR3bits.U1TXIP = 0;
    U1CON1bits.ON = 1;
    U1CON0bits.TXEN = 1;
    state = WAIT_START1;
    bytesNeeded = 1;
    lastCommand = RX_IDLE;
    txBytes = 0;
    txSource = NULL;
    U1CON0bits.RXEN = 1;
}

//void txBytes(void *buf, int nBytes) {
//    uint8_t *p = (uint8_t *) buf;
//    while (nBytes > 0) {
//        while (PIR3bits.U1TXIF == 0);
//        U1TXB = *p;
//        ++p;
//        --nBytes;
//    }
//}

void __interrupt(irq(U1RX), base(8)) U1_RX_ISR() {
    uint8_t rx;
    while (PIR3bits.U1RXIF) {
        rx = U1RXB;
        switch (state) {
            case RX_IDLE:
                break;
            case WAIT_START1:
                if (rx == 0x4d) {
                    state = WAIT_START2;
                }
                break;
            case WAIT_START2:
                if (rx == 0x63) {
                    txBuf[0] = VERSION_MAJOR;
                    txBuf[1] = VERSION_MINOR;
                    txBuf[2] = (uint8_t)MAX_MEMORY;
                    txBuf[3] = (uint8_t)(MAX_MEMORY >> 8);
                    txBytes = 4;
                    txSource = txBuf;
                    txStart();
                    state = WAIT_START1;
                } else {
                    state = WAIT_START1;
                }
                break;
        }
    }
}

void __interrupt(irq(U1TX), base(8)) U1_TX_ISR() {
    while (txBytes > 0 && PIR3bits.U1TXIF == 1) {
        U1TXB = *txSource;
        --txBytes;
        ++txSource;
    }
    if (txBytes == 0) {
        txStop();
        txSource = NULL;
        ledOn();
    }
}