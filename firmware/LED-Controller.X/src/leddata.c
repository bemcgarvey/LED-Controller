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
        ledOn();
    } else {
        copyFromROM();
    }
    calculatePointers();
}

char copyToROM(uint16_t size) {
    int bytesRemaining;
    bytesRemaining = (int) size;
    INTCON0bits.GIE = 0; //Disable interrupts for the duration of the copy
    uint8_t *src = controller.bytes;
    uint32_t dest = (uint32_t) & controllerROM;
    uint16_t checksum = 0;
    for (int i = 0; i < size; ++i) {
        checksum += controller.bytes[i];
    }
    while (bytesRemaining > 0) {
        //Erase page
        NVMCON1bits.REG = 0b10;
        NVMCON1bits.FREE = 1;
        TBLPTRU = (dest & 0xff0000) >> 16;
        TBLPTRH = (dest & 0x00ff00) >> 8;
        TBLPTRL = (dest & 0x0000ff);
        NVMCON1bits.WREN = 1;
        NVMCON2 = 0x55;
        NVMCON2 = 0xaa;
        NVMCON1bits.WR = 1;
        NVMCON1bits.FREE = 0;
        if (bytesRemaining == size) {
            //First page include size and checksum
            TABLAT = size & 0xff;
            asm("TBLWT*+");
            TABLAT = (size >> 8) & 0xff;
            asm("TBLWT*+");
            TABLAT = checksum & 0xff;
            asm("TBLWT*+");
            TABLAT = (checksum >> 8) & 0xff;
            asm("TBLWT*+");
            for (int i = 0; i < 124; ++i) {
                TABLAT = *src;
                if (i < 123) {
                    asm("TBLWT*+");
                } else {
                    asm("TBLWT*");
                }
                ++src;
                --bytesRemaining;
            }
        } else {
            for (int i = 0; i < 128; ++i) {
                TABLAT = *src;
                if (i < 127) {
                    asm("TBLWT*+");
                } else {
                    asm("TBLWT*");
                }
                ++src;
                --bytesRemaining;
            }
        }
        NVMCON1bits.WREN = 1;
        NVMCON2 = 0x55;
        NVMCON2 = 0xaa;
        NVMCON1bits.WR = 1;
        dest += 128;
    }
    NVMCON1bits.WREN = 0;
    INTCON0bits.GIE = 1;
    for (int i = 0; i < size; ++i) {
        if (controller.bytes[i] != controllerROM.controller.bytes[i]) {
            return 0;
        }
    }
    return 1;
}

void copyFromROM(void) {
    controllerSize = controllerROM.size;
    for (int i = 0; i < controllerSize; ++i) {
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

