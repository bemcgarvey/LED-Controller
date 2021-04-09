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
    int8_t step;
} OutputAction;

OutputAction actions[6];
int8_t currentRCRange = -1;

void processAction(uint8_t out);

enum {
    PATTERN_A = 0x00, PATTERN_B = 0x01, PATTERN_C = 0x02, ROTATE_OUT = 0x40
    , ROTATE_IN = 0x80, BOUNCE = 0xC0
};

uint8_t activePattern = PATTERN_A;

void initActions(void) {
    //Start with Pattern A
    for (char i = 0; i < 6; ++i) {
        if (outputs[i]->numLEDs > 0) {
            actions[i].timeCountsRemaining = patterns[3 * i]->onTime;
            actions[i].nextPattern = patterns[3 * i]->nextPattern;
            actions[i].startLED = 0;
            actions[i].currentPattern = PATTERN_A;
            actions[i].step = actions[i].nextPattern & 0x3f;
            if ((actions[i].nextPattern & 0xc0) == ROTATE_OUT || (actions[i].nextPattern & 0xc0) == BOUNCE) {
                actions[i].step = -actions[i].step;
            }
        } else {
            actions[i].timeCountsRemaining = 0;
        }
    }
    currentRCRange = -1;
    activePattern = PATTERN_A;
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
            actions[out].step = actions[out].nextPattern & 0x3f;
            if ((actions[out].nextPattern & 0xc0) == ROTATE_OUT || (actions[out].nextPattern & 0xc0) == BOUNCE) {
                actions[out].step = -actions[out].step;
            }
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
    if (actions[out].nextPattern > PATTERN_C) {
        pat = patterns[out * 3 + actions[out].currentPattern];
    } else {
        pat = patterns[out * 3 + actions[out].nextPattern];
    }
    if (pat != NULL) {
        if (actions[out].nextPattern & 0xc0) {
            int newStart = actions[out].startLED;
            newStart += actions[out].step;
            if ((actions[out].nextPattern & 0xc0) == BOUNCE) {
                if (actions[out].step > 0 && newStart >= pat->numLEDs) {
                    actions[out].step = -actions[out].step;
                } else if (actions[out].step < 0 && newStart == -actions[out].step) {
                    actions[out].step = -actions[out].step;
                }
            }
            if (newStart >= pat->numLEDs) {
                newStart -= pat->numLEDs;
            } else if (newStart < 0) {
                newStart += pat->numLEDs;
            }
            actions[out].startLED = (uint8_t) newStart;
            setLEDs(out, &(pat->rgbs), pat->numLEDs, (uint8_t) newStart);
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
            updateNewPattern(PATTERN_A);
            break;
        case RC_PATTERN_B:
            updateNewPattern(PATTERN_B);
            break;
        case RC_PATTERN_C:
            updateNewPattern(PATTERN_C);
            break;
        case RC_NEXT_PATTERN:
            if (activePattern == PATTERN_C) {
                updateNewPattern(PATTERN_A);
            } else {
                updateNewPattern(activePattern + 1);
            }
            break;
        default:
            break;
    }
}
