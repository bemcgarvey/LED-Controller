/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: leddata.h                                 //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: LED data structures                //
/////////////////////////////////////////////////////

#ifndef LEDDATA_H
#define	LEDDATA_H

#include <stdint.h>

#define MAX_MEMORY 3584

typedef struct {
    uint8_t numLEDs;
    int16_t onTime;
    uint8_t nextPattern;
    uint8_t rgbs;
} LEDPattern;

typedef struct {
    uint8_t numLEDs;
    uint8_t numPatterns;
    LEDPattern patterns;
} Output;

typedef struct {
    uint8_t numOutputs;
    uint8_t actions[6];
    Output outputs;
} ControllerConfig;

typedef union {
    uint8_t bytes[MAX_MEMORY];
    ControllerConfig config;
} Controller;

typedef struct {
    uint16_t size;
    uint16_t checksum;
    Controller controller;
} ControllerROM;

extern Controller controller;
extern const ControllerROM controllerROM;
extern const uint8_t fallbackControllerROM[];
extern Output* outputs[6];
extern LEDPattern* patterns[18];
extern uint16_t controllerSize;

void initControllerMemory(void);
char copyToROM(uint16_t size);
void copyFromROM(void);
void calculatePointers(void);
char validateROM(void);

#endif	/* LEDDATA_H */

