/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: actions.c                                 //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: RC and timing actions              //
/////////////////////////////////////////////////////

#include <xc.h>
#include "actions.h"
#include "leddata.h"
#include "leds.h"
#include "capture.h"

typedef struct {
    int16_t timeCountsRemaining;
    uint8_t currentPattern;
    uint8_t nextPattern;
    uint8_t startLED;
    int8_t direction;
} OutputAction;

OutputAction actions[6];
int8_t currentRCRange = -1;

void processAction(uint8_t out);

enum {
    PAT_A = 0, PAT_B = 1, PAT_C = 2, LAST_REGULAR_PATTERN = 2
    , BOUNCE = 253, ROTATE_OUT = 254, ROTATE_IN = 255
};
uint8_t activePattern = PAT_A;

void initActions(void) {
    //Start with PatternA
    for (char i = 0; i < 6; ++i) {
        if (outputs[i]->numLEDs > 0) {
            actions[i].timeCountsRemaining = patterns[3 * i]->onTime;
            actions[i].nextPattern = patterns[3 * i]->nextPattern;
            actions[i].startLED = 0;
            actions[i].currentPattern = PAT_A;
            actions[i].direction = -1;
        } else {
            actions[i].timeCountsRemaining = 0;
        }
    }
    currentRCRange = -1;
    activePattern = PAT_A;
}

void updateNewPattern(uint8_t newPattern) {
    for (char out = 0; out < 6; ++out) {
        LEDPattern *pat = patterns[out * 3 + newPattern];
        if (pat != NULL) {
            setLEDs(out, &(pat->rgbs), outputs[out]->numLEDs, 0);
            actions[out].timeCountsRemaining = pat->onTime;
            actions[out].nextPattern = pat->nextPattern;
            actions[out].startLED = 0;
            actions[out].currentPattern = newPattern;
            actions[out].direction = -1;
        } else {
            clearLEDs(out, outputs[out]->numLEDs);
            actions[out].timeCountsRemaining = 0;
        }
    }
    activePattern = newPattern;
}

void doTimeTick(void) {
    for (char i = 0; i < 6; ++i) {
        if (actions[i].timeCountsRemaining > 0) {
            --actions[i].timeCountsRemaining;
            if (actions[i].timeCountsRemaining == 0) {
                processAction(i);
            }
        }
    }
}

void processAction(uint8_t out) {
    LEDPattern *pat;
    if (actions[out].nextPattern > LAST_REGULAR_PATTERN) {
        pat = patterns[out * 3 + actions[out].currentPattern];
    } else {
        pat = patterns[out * 3 + actions[out].nextPattern];
    }
    if (pat != NULL) {
        if (actions[out].nextPattern == ROTATE_IN) {
            uint8_t start = actions[out].startLED;
            ++actions[out].startLED;
            if (actions[out].startLED >= pat->numLEDs) {
                actions[out].startLED = 0;
            }
            setLEDs(out, &(pat->rgbs), pat->numLEDs, start);
            actions[out].timeCountsRemaining = pat->onTime;
        } else if (actions[out].nextPattern == ROTATE_OUT) {
            uint8_t start = actions[out].startLED;
            if (actions[out].startLED == 0) {
                actions[out].startLED = pat->numLEDs - 1;
            } else {
                --actions[out].startLED;
            }
            setLEDs(out, &(pat->rgbs), pat->numLEDs, start);
            actions[out].timeCountsRemaining = pat->onTime;
        } else if (actions[out].nextPattern == BOUNCE) {
            uint8_t start = actions[out].startLED;
            if (start == 255) {
                start = pat->numLEDs - 1;
            } else if (start == pat->numLEDs) {
                start = 0;
                actions[out].direction = -1;
            } else if (start == 1) {
                actions[out].direction = 1;
            }
            actions[out].startLED = start;
            actions[out].startLED += actions[out].direction;
            setLEDs(out, &(pat->rgbs), pat->numLEDs, start);
            actions[out].timeCountsRemaining = pat->onTime;
        } else {
            setLEDs(out, &(pat->rgbs), outputs[out]->numLEDs, 0);
            actions[out].timeCountsRemaining = pat->onTime;
            actions[out].nextPattern = pat->nextPattern;
        }
    } else {
        clearLEDs(out, outputs[out]->numLEDs);
    }
}

int8_t decodeRCRange(void) {
    int us = currentPWMus();
    if (us < 1151) {
        return 0;
    } else if (us < 1351) {
        return 1;
    } else if (us < 1551) {
        return 2;
    } else if (us < 1751) {
        return 3;
    } else if (us < 1951) {
        return 4;
    } else {
        return 5;
    }
}

void processRCAction(void) {
    switch (controller.config.actions[currentRCRange]) {
        case RC_DO_NOTHING:
            break;
        case RC_OFF:
            for (uint8_t i = 0; i < 6; ++i) {
                clearLEDs(i, outputs[i]->numLEDs);
                actions[i].timeCountsRemaining = 0;
            }
            break;
        case RC_PATTERN_A:
            updateNewPattern(PAT_A);
            break;
        case RC_PATTERN_B:
            updateNewPattern(PAT_B);
            break;
        case RC_PATTERN_C:
            updateNewPattern(PAT_C);
            break;
        case RC_NEXT_PATTERN:
            if (activePattern == LAST_REGULAR_PATTERN) {
                updateNewPattern(PAT_A);
            } else {
                updateNewPattern(activePattern + 1);
            }
            break;
        default:
            break;
    }
}
