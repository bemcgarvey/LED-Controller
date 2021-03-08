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
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC0 = 1;

    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCKbits.PPSLOCKED = 0;
    RA1PPS = 0b000001; //CLC1OUT
    RA2PPS = 0b000010; //CLC2OUT
    //RB3PPS = 0b011111; //SPI SDO
    RC2PPS = 0b010011; //UART1 TX
    U1RXPPS = 0b010011; //RC3
    T1GPPS = 0b010000; //RC0
    
    //Keep unlocked
    //PPSLOCK = 0x55;
    //PPSLOCK = 0xaa;
    //PPSLOCKbits.PPSLOCKED = 1;
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
