/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: system.c                                  //
// Target: PIC18F2xK42                             // 
// Compiler: XC8                                   //
// Author: Brad McGarvey                           //
// License: GNU General Public License v3.0        //
// Description: system configuration functions     //
/////////////////////////////////////////////////////

#include <xc.h>
#include "system.h"

void initPins(void) {
    ANSELA = 0; //All digital
    ANSELB = 0;
    ANSELC = 0;
    LATA = 0;
    LATB = 0;
    LATC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;

    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCKbits.PPSLOCKED = 0;
    /*RB2PPS = 0x04;   //RB2->CLC4:CLC4;    
    CCP1PPS = 0x10;   //RC0->CCP1:CCP1;    
    RB3PPS = 0x03;   //RB3->CLC3:CLC3;    
    RC2PPS = 0x20;   //RC2->UART1:TX1;    
    RA1PPS = 0x01;   //RA1->CLC1:CLC1;    
    RA2PPS = 0x02;   //RA2->CLC2:CLC2;    
    RC5PPS = 0x06;   //RC5->CLC6:CLC6;    
    RC6PPS = 0x05;   //RC6->CLC5:CLC5;    
    U1RXPPS = 0x13;   //RC3->UART1:RX1;*/
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCKbits.PPSLOCKED = 1;
}

void initOscillator(void) {
    //adjust HFINTOSC if needed
}

void initPMD(void) {

}

void initInterrupts(void) {
    INTCON0bits.IPEN = 1;
    GIE = 0;
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0; // unlock IVT
    IVTBASEU = 0;
    IVTBASEH = 0;
    IVTBASEL = 8;
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 1; // lock IVT

}

void __interrupt(irq(default),base(8)) Default_ISR()
{
}

inline void ledOn(void) {
    LATCbits.LATC1 = 1;
}

inline void ledOff(void) {
    LATCbits.LATC1 = 0;
}

inline void ledToggle(void) {
    LATCbits.LATC1 ^= 1;
}