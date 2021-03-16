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
    RX_IDLE, WAIT_START1, WAIT_START2, WAIT_COMMAND, WAIT_DATA_SIZE, WAIT_DATA
};

enum SerialCommands {
    CMD_READ = 0x80, CMD_WRITE = 0x81, CMD_TEST = 0x82
};

enum SerialRespnses {
    ACK = 0x01, NACK = 0x00
};
volatile enum rxState state;
volatile int bytesNeeded;
volatile enum rxState lastCommand;
volatile uint8_t *rxDestination;
volatile uint8_t tempRxBuf[2];
volatile uint8_t txHeader[4];
volatile uint8_t txHeaderBytes;
volatile uint8_t *txHeaderPos;
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
                    txHeader[0] = VERSION_MAJOR;
                    txHeader[1] = VERSION_MINOR;
                    txHeader[2] = (uint8_t) MAX_MEMORY;
                    txHeader[3] = (uint8_t) (MAX_MEMORY >> 8);
                    txHeaderBytes = 4;
                    txHeaderPos = txHeader;
                    txBytes = 0;
                    txStart();
                    state = WAIT_COMMAND;
                } else {
                    state = WAIT_START1;
                }
                break;
            case WAIT_COMMAND:
                if (rx == CMD_WRITE) {
                    state = WAIT_DATA_SIZE;
                    lastCommand = rx;
                    bytesNeeded = 2;
                    rxDestination = tempRxBuf;
                } else if (rx == CMD_READ) {
                    txSource = controller.bytes;
                    txHeaderBytes = 2;
                    *((uint16_t *)txHeader) = calculateSize();
                    txBytes = *((uint16_t *)txHeader);
                    txHeaderPos = txHeader;
                    txStart();
                    state = WAIT_COMMAND;  //TODO change to WAIT_TMT?? Don't accept commands while transmitting?
                } else if (rx == CMD_TEST) {

                } else {
                    state = WAIT_COMMAND;
                }
                break;
            case WAIT_DATA_SIZE:
                *rxDestination = rx;
                ++rxDestination;
                --bytesNeeded;
                if (bytesNeeded == 0) {
                    rxDestination = controller.bytes;
                    bytesNeeded = *((uint8_t *) tempRxBuf);
                    state = WAIT_DATA;
                }
            case WAIT_DATA:
                *rxDestination = rx;
                ++rxDestination;
                --bytesNeeded;
                if (bytesNeeded == 0) {
                    state = WAIT_COMMAND;
                }
        }
    }
}

void __interrupt(irq(U1TX), base(8)) U1_TX_ISR() {
    while (txHeaderBytes > 0 && PIR3bits.U1TXIF) {
        U1TXB = *txHeaderPos;
        ++txHeaderPos;
        --txHeaderBytes;
    }
    if (txHeaderBytes == 0) {
        while (txBytes > 0 && PIR3bits.U1TXIF == 1) {
            U1TXB = *txSource;
            --txBytes;
            ++txSource;
        }
        if (txBytes == 0) {
            txStop();
            txSource = NULL;
        }
    }
}