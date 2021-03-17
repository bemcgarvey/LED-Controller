#include <xc.h>
#include "leddata.h"

Controller controller;
Output* outputs[6];
LEDPattern* patterns[18];

void initControllerMemory(void) {
    copyFromROM();
    calculatePointers();
}

void copyToROM(void) {
    
}

void copyFromROM(void) {
    for (int i = 0; i < MAX_MEMORY; ++i) {
        controller.bytes[i] = controllerROM.bytes[i];
    }
}

uint16_t calculateSize(void) {
    uint16_t size = 7;
    for (int i = 0; i < 6; ++i) {
        size += 2;
        size += outputs[i]->numPatterns * 4;
        size += outputs[i]->numPatterns * outputs[i]->numLEDs * 3;
    }
    return size;
}

void calculatePointers(void) {
    for (int i = 0; i < 18; ++i) {
        patterns[i] = NULL;
    }
    uint8_t *pc = controller.bytes;
    pc += 7;
    for (int i = 0; i < 6; ++i) {
        Output *op = (Output *)pc;
        outputs[i] = op;
        pc += 2;
        for (int p = 0; p < op->numPatterns; ++p) {
            LEDPattern *pp = (LEDPattern *)pc;
            patterns[i * 3 + p] = pp;
            pc += 4 + (pp->numLEDs * 3);
        }
    }
}

const Controller controllerROM __at(0xf000) = {
    6,  //numOutputs
    0, 0, 0, 0, 0, 0,  //actions
    0, 1, /*output 1*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 2*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 3*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 4*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 5*/ 0, 0xff, 0xff, 0, //Pattern 1
    0, 1, /*output 6*/ 0, 0xff, 0xff, 0, //Pattern 1
};