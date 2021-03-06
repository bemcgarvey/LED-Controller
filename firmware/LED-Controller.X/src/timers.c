#include <xc.h>
#include "timers.h"


void initTimer1(void) {
    T1CONbits.ON = 0;
    T1CONbits.RD16 = 1;
    T1CONbits.CKPS = 0b11; //1:8
    T1CLK = 0b00001; //Fosc/4
    //T1 has 2us period
    T1GCONbits.GE = 1;
    T1GCONbits.GPOL = 1;
    T1GCONbits.GSPM = 1;
    T1GCONbits.GTM = 0;
    T1GATE = 0b0000;
    PIR4bits.TMR1GIF = 0;
    IPR4bits.TMR1GIP = 1;
    PIE4bits.TMR1GIE = 1;
    TMR1H = 0;
    TMR1L = 0;
    T1CONbits.ON = 1;
}