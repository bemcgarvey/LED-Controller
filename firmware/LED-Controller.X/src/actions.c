#include <xc.h>
#include "actions.h"
#include "leddata.h"
#include "leds.h"

OutputAction actions[6];

void initActions(void) {
    for (char i = 0; i < 6; ++i) {
        if (outputs[i]->numLEDs > 0) {
            actions[i].timeCountsRemaining = patterns[3 * i]->onTime;
            actions[i].nextPattern = patterns[3 * i]->nextPattern;
            actions[i].active = 1;
            setLEDs(i,  &(patterns[i * 3]->rgbs), outputs[i]->numLEDs);
        } else {
            actions[i].active = 0;
        }
    }
}

void updateActions(uint8_t newPattern) {
    
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
        setLEDs(out,  &(pat->rgbs), outputs[out]->numLEDs);
        actions[out].timeCountsRemaining = pat->onTime;
        actions[out].nextPattern = pat->nextPattern;
    } else {
        clearLEDs(out, outputs[out]->numLEDs);
    }
}