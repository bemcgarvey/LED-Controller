/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: timers.c                                  //
// Target: PIC18F2xQ43                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: Timer functions                    //
/////////////////////////////////////////////////////

#include <xc.h>
#include "timers.h"
#include <stdint.h>
#include "system.h"

unsigned int systemTime = 0;

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
    PIR3bits.TMR1GIF = 0;
    IPR3bits.TMR1GIP = 1;
    PIE3bits.TMR1GIE = 1;
    TMR1H = 0;
    TMR1L = 0;
    T1CONbits.ON = 1;
}

void initSystemTimer(void) {
    T0CON0bits.EN = 0;
    T0CON0bits.MD16 = 0;
    T0CON0bits.OUTPS = 0b1001; // 1:10 postscale
    T0CON1bits.CS = 0b010;  //Fosc/4
    T0CON1bits.CKPS = 0b0110; // 1:64 prescale
    TMR0L = 0;
    TMR0H = 250;  // 16000000/64/250 = 1000 Hz / 10 = 100 Hz So timer period is 0.01 s
    PIR3bits.TMR0IF = 0;
    IPR3bits.TMR0IP = 0;
    PIE3bits.TMR0IE = 1;
    systemTime = 0;
    T0CON0bits.EN = 1;  
}

void __interrupt(irq(TMR0), low_priority, base(8)) Timer0_ISR() {
    static uint8_t count = 0;
    ++count;
    if (count == 10) {
        count = 0;
        ++systemTime;
    }
    PIR3bits.TMR0IF = 0;
}