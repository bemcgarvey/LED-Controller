/* 
 * File:   leddata.h
 * Author: bemcg
 *
 * Created on March 6, 2021, 1:07 PM
 */

#ifndef LEDDATA_H
#define	LEDDATA_H

#include <stdint.h>

#define MAX_MEMORY 3584

typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} LEDrgb;

typedef struct {
    uint8_t numLEDs;
    int16_t onTime;
    uint8_t nextPattern;
    LEDrgb rgbs;
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

extern Controller controller;
extern const Controller controllerROM;
extern Output* outputs[6];
extern LEDPattern* patterns[18];

void initControllerMemory(void);
void copyToROM(void);
void copyFromROM(void);
uint16_t calculateSize(void);
void calculatePointers(void);

#endif	/* LEDDATA_H */

