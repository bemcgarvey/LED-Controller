/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leddata.c                                 //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: LED data structures                //
/////////////////////////////////////////////////////

#include <xc.h>
#include "leddata.h"
#include "system.h"

Controller controller;
Output* outputs[6];
LEDPattern* patterns[18];
uint16_t controllerSize;

const uint8_t fallbackControllerROM[] = {
    6, //numOutputs
    0, 0, 0, 0, 0, 0, //actions
    0, 1, /*output 1*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 2*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 3*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 4*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 5*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 6*/ 0, 0xff, 0xff, 0, //Pattern 1
};

void initControllerMemory(void) {
    if (!validateROM()) {
        for (int i = 0; i < sizeof (fallbackControllerROM); ++i) {
            controller.bytes[i] = fallbackControllerROM[i];
        }
        controllerSize = sizeof (fallbackControllerROM);
        for (int i = 0; i < 6; ++i) {
            ledToggle();  //Blink led 3 times to indicate invalid ROM
            __delay_ms(300);
        }
    } else {
        copyFromROM();
    }
    calculatePointers();
}

#if defined __18F26Q43
uint8_t NVMBuffer[256] __at(0x1500);
#elif defined __18F27Q43
uint8_t NVMBuffer[256] __at(0x2500);
#else
#error "Invalid processor.  Must be PIC18F26Q43 or PIC18F27Q43"
#endif

char copyToROM(uint16_t size) {
    uint16_t bytesRemaining;
    char firstPage = 1;
    bytesRemaining = size;
    INTCON0bits.GIE = 0; //Disable interrupts for the duration of the copy
    uint8_t *src = controller.bytes;
    uint24_t dest = (uint24_t) &controllerROM;
    uint16_t checksum = 0;
    for (int i = 0; i < size; ++i) {
        checksum += controller.bytes[i];
    }
    while (bytesRemaining > 0) {
        //Erase page
        NVMADR = dest;
        NVMCON1bits.NVMCMD = 0b110; 
        NVMLOCK = 0x55;
        NVMLOCK = 0xaa;
        NVMCON0bits.GO = 1;
        while (NVMCON0bits.GO == 1);
        uint8_t *pageBuffer = NVMBuffer;
        if (firstPage) {
            //First page include size and checksum
            firstPage = 0;
            *pageBuffer++ = (uint8_t)(size & 0xff);
            *pageBuffer++ = (uint8_t)((size >> 8) & 0xff);
            *pageBuffer++ = (uint8_t)(checksum & 0xff);
            *pageBuffer++ = (uint8_t)((checksum >> 8) & 0xff);
            for (int i = 0; i < 252; ++i) {
                *pageBuffer++ = *src;
                ++src;
                --bytesRemaining;
                if (bytesRemaining == 0) {
                    break;
                }
            }
        } else {
            for (int i = 0; i < 256; ++i) {
                *pageBuffer++ = *src;
                ++src;
                --bytesRemaining;
                if (bytesRemaining == 0) {
                    break;
                }
            }
        }
        NVMCON1bits.NVMCMD = 0b101;
        NVMLOCK = 0x55;
        NVMLOCK = 0xaa;
        NVMCON0bits.GO = 1;
        while (NVMCON0bits.GO == 1);
        dest += 256;
    }
    NVMCON1bits.NVMCMD = 0b000;
    INTCON0bits.GIE = 1;
    for (int i = 0; i < size; ++i) {
        if (controller.bytes[i] != controllerROM.controller.bytes[i]) {
            ledOff();
            return 0;
        }
    }
    return 1;
}

void copyFromROM(void) {
    controllerSize = controllerROM.size;
    for (uint16_t i = 0; i < controllerSize; ++i) {
        controller.bytes[i] = controllerROM.controller.bytes[i];
    }
}

void calculatePointers(void) {
    for (int i = 0; i < 18; ++i) {
        patterns[i] = NULL;
    }
    uint8_t *pc = controller.bytes;
    pc += 7;
    for (int i = 0; i < 6; ++i) {
        Output *op = (Output *) pc;
        outputs[i] = op;
        pc += 2;
        for (int p = 0; p < op->numPatterns; ++p) {
            LEDPattern *pp = (LEDPattern *) pc;
            patterns[i * 3 + p] = pp;
            pc += 4 + (pp->numLEDs * 3);
        }
    }
}

char validateROM(void) {
    if (controllerROM.size > MAX_MEMORY) {
        return 0;
    }
    uint16_t check = 0;
    for (int i = 0; i < controllerROM.size; ++i) {
        check += controllerROM.controller.bytes[i];
    }
    if (check != controllerROM.checksum) {
        return 0;
    }
    return 1;
}

const ControllerROM controllerROM __at(0xe000) = {
    43, 3072,
    {
        6, //numOutputs
        0, 0, 0, 0, 0, 0, //actions
        0, 1, /*output 1*/ 0, 0xff, 0xff, 0, //Pattern 1
        0, 1, /*output 2*/ 0, 0xff, 0xff, 0, //Pattern 1
        0, 1, /*output 3*/ 0, 0xff, 0xff, 0, //Pattern 1
        0, 1, /*output 4*/ 0, 0xff, 0xff, 0, //Pattern 1
        0, 1, /*output 5*/ 0, 0xff, 0xff, 0, //Pattern 1
        0, 1, /*output 6*/ 0, 0xff, 0xff, 0
    } //Pattern 1
};

