#include <xc.h>
#include <stdint.h>
#include "capture.h"
#include "timers.h"
#include "system.h"

uint16_t currentPWMInput = 0;

void initCapture(void) {
    currentPWMInput = 0;
    initTimer1();
    T1GCONbits.GGO = 1;
}

int currentPWMus(void) {
    INTCON0bits.GIEH = 0;
    uint16_t lastPWM = currentPWMInput;
    INTCON0bits.GIEH = 1;
    lastPWM /= 2;
    return (int)lastPWM;
}

void __interrupt(irq(TMR1G), high_priority, base(8)) Capture_ISR() {
    currentPWMInput = *((uint16_t *) &TMR1L);
    TMR1H = 0;
    TMR1L = 0;
    PIR4bits.TMR1GIF = 0;
    T1GCONbits.GGO = 1;
}

