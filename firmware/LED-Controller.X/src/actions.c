#include <xc.h>
#include "actions.h"
#include "leddata.h"
#include "leds.h"
#include "capture.h"

OutputAction actions[6];
int8_t currentRCRange = -1;

enum {PAT_A = 0, PAT_B, PAT_C};
uint8_t activePattern = PAT_A;

void initActions(void) {
    //Start with PatternA
    for (char i = 0; i < 6; ++i) {
        if (outputs[i]->numLEDs > 0) {
            actions[i].timeCountsRemaining = patterns[3 * i]->onTime;
            actions[i].nextPattern = patterns[3 * i]->nextPattern;
            actions[i].active = 1;
        } else {
            actions[i].active = 0;
        }
    }
    currentRCRange = -1;
    activePattern = PAT_A;
}

void updateNewPattern(uint8_t newPattern) {
    for (char out = 0; out < 6; ++out) {
        LEDPattern *pat = patterns[out * 3 + newPattern];
        if (pat != NULL) {
            setLEDs(out, &(pat->rgbs), outputs[out]->numLEDs);
            actions[out].timeCountsRemaining = pat->onTime;
            actions[out].nextPattern = pat->nextPattern;
        } else {
            clearLEDs(out, outputs[out]->numLEDs);
        }
    }
    activePattern = newPattern;
}

void doTimeTick(void) {
    for (char i = 0; i < 6; ++i) {
        if (actions[i].active && actions[i].timeCountsRemaining > 0) {
            --actions[i].timeCountsRemaining;
            if (actions[i].timeCountsRemaining == 0) {
                processAction(i);
            }
        }
    }
}

void processAction(uint8_t out) {
    LEDPattern *pat = patterns[out * 3 + actions[out].nextPattern];
    if (pat != NULL) {
        setLEDs(out, &(pat->rgbs), outputs[out]->numLEDs);
        actions[out].timeCountsRemaining = pat->onTime;
        actions[out].nextPattern = pat->nextPattern;
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
            if (activePattern == PAT_C) {
                updateNewPattern(PAT_A);
            } else {
                updateNewPattern(activePattern + 1);
            }
            break;
        default:
            break;
    }
}
