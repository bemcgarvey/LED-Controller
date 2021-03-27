/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: actions.h                                 //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: RC and time based actions          //
/////////////////////////////////////////////////////

#ifndef ACTIONS_H
#define	ACTIONS_H

#include <stdint.h>

typedef struct {
    uint8_t active;
    int16_t timeCountsRemaining;
    uint8_t nextPattern;
} OutputAction;

extern OutputAction actions[6];

enum RCActions {RC_DO_NOTHING = 0, RC_OFF, RC_PATTERN_A, RC_PATTERN_B, RC_PATTERN_C, RC_NEXT_PATTERN};
extern int8_t currentRCRange; 

void initActions(void);
void doTimeTick(void);
void processAction(uint8_t out);
int8_t decodeRCRange(void);
void processRCAction(void);

#endif	/* ACTIONS_H */

