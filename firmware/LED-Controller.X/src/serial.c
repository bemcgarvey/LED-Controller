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
    CMD_NONE = 0, CMD_READ = 0x80, CMD_WRITE = 0x81, CMD_TEST = 0x82
};

enum SerialRespnses {
    ACK = 0x06, NACK = 0x15
};
volatile enum rxState state;
volatile uint16_t bytesNeeded;
volatile enum SerialCommands lastCommand;  //TODO is this needed?
volatile uint8_t *rxDestination;
volatile uint8_t tempRxBuf[2];
volatile uint8_t txHeader[4];
volatile uint8_t txHeaderBytes;
volatile uint8_t *txHeaderPos;
volatile uint16_t txBytes;
volatile uint8_t *txSource;
volatile uint8_t sendChecksum;

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
    U1CON2bits.C0EN = 1; //enable checksum
    sendChecksum = 0;
    U1CON1bits.ON = 1;
    U1CON0bits.TXEN = 1;
    state = WAIT_START1;
    bytesNeeded = 1;
    lastCommand = CMD_NONE;
    txBytes = 0;
    txSource = NULL;
    U1CON0bits.RXEN = 1;
}


void __interrupt(irq(U1RX), low_priority, base(8)) U1_RX_ISR() {
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
                    lastCommand = CMD_NONE;
                    txHeader[0] = VERSION_MAJOR;
                    txHeader[1] = VERSION_MINOR;
                    txHeader[2] = (uint8_t) MAX_MEMORY;
                    txHeader[3] = (uint8_t) (MAX_MEMORY >> 8);
                    txHeaderBytes = 4;
                    txHeaderPos = txHeader;
                    txBytes = 0;
                    sendChecksum = 0;
                    txStart();
                    state = WAIT_COMMAND;
                } else {
                    state = WAIT_START1;
                }
                break;
            case WAIT_COMMAND:
                if (rx == CMD_WRITE) {
                    state = WAIT_DATA_SIZE;
                    lastCommand = CMD_WRITE;
                    bytesNeeded = 2;
                    rxDestination = tempRxBuf;
                } else if (rx == CMD_READ) {
                    lastCommand = CMD_READ;
                    txSource = controller.bytes;
                    txHeaderBytes = 2;
                    *((uint16_t *)txHeader) = calculateSize();
                    txBytes = *((uint16_t *)txHeader);
                    txHeaderPos = txHeader;
                    sendChecksum = 1;
                    txStart();
                    state = WAIT_COMMAND;
                } else if (rx == CMD_TEST) {

                } else if (rx == 0x4d) {
                    state = WAIT_START2;
                } else {
                    state = WAIT_COMMAND;
                }
                break;
            case WAIT_DATA_SIZE:
                *rxDestination = rx;
                ++rxDestination;
                --bytesNeeded;
                if (bytesNeeded == 0) {
                    U1RXCHK = 0;
                    rxDestination = controller.bytes;
                    bytesNeeded = *((uint16_t *) tempRxBuf);
                    ++bytesNeeded;  //Extra byte for checksum
                    state = WAIT_DATA;
                }
                break;
            case WAIT_DATA:
                if (bytesNeeded > 1) {  //don't store checksum
                    *rxDestination = rx;
                }
                ++rxDestination;
                --bytesNeeded;
                if (bytesNeeded == 0) {
                    if (U1RXCHK == 1) {  //Last carry should result in a 1
                        if (copyToROM(*(uint16_t*)tempRxBuf) != 0) {
                            U1TXB = ACK;
                        } else {
                            U1TXB = NACK;
                        }
                    } else {
                        U1TXB = NACK;
                        copyFromROM();
                    }
                    calculatePointers();
                    state = WAIT_COMMAND;
                }
                break;
        }
    }
}

void __interrupt(irq(U1TX), low_priority, base(8)) U1_TX_ISR() {
    while (txHeaderBytes > 0 && PIR3bits.U1TXIF) {
        U1TXB = *txHeaderPos;
        ++txHeaderPos;
        --txHeaderBytes;
        if (txHeaderBytes == 0) {
            U1TXCHK = 0;  //clear checksum
        }
    }
    if (txHeaderBytes == 0) {
        while (txBytes > 0 && PIR3bits.U1TXIF == 1) {
            U1TXB = *txSource;
            --txBytes;
            ++txSource;
        }
        if (txBytes == 0) {
            if (sendChecksum && PIR3bits.U1TXIF == 1) {
                U1TXB = -U1TXCHK;
                sendChecksum = 0;
            }
            if (!sendChecksum) {
                txStop();
                txSource = NULL;
            }
        }
    }
}