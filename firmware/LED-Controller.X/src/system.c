/////////////////////////////////////////////////////
// Project: LED-Controller                         //
// File: system.c                                  //
// Target: PIC18F2xQ43                             // 
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
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC0 = 1;
    //PPS
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCKbits.PPSLOCKED = 0;
    RA1PPS = 0x01; //CLC1OUT
    RA2PPS = 0x02; //CLC2OUT
    RB3PPS = 0x03; //CLC3OUT
    RB2PPS = 0x04; //CLC4OUT
    RC6PPS = 0x05; //CLC5OUT
    RC5PPS = 0x06; //CLC6OUT
    RC2PPS = 0x20; //UART1TX
    U1RXPPS = 0b010011; //RC3
    T1GPPS = 0b010000; //RC0
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCKbits.PPSLOCKED = 1;
}

void initOscillator(void) {
    //adjust HFINTOSC if needed
    while (OSCSTATbits.HFOR != 1);  //Wait for HFINTOSC ready
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
