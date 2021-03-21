/* 
 * File:   actions.h
 * Author: bemcg
 *
 * Created on March 20, 2021, 8:34 PM
 */

#ifndef ACTIONS_H
#define	ACTIONS_H

#include <stdint.h>

typedef struct {
    uint8_t active;
    int16_t timeCountsRemaining;
    uint8_t nextPattern;
} OutputAction;

extern OutputAction actions[6];

void initActions(void);
void updateActions(uint8_t newPattern);
void doTimeTick(void);
void processAction(uint8_t out);

#endif	/* ACTIONS_H */

