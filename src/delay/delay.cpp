
#include "delay.h"

// DriverLib
#include <msp430.h>     // __delay_cycles



/*
 * Only for 1Mhz cpu clock speed.
 */

void Delay::oneMicrosecond()     { __delay_cycles(1);}
void Delay::hundredMicroseconds(){ __delay_cycles(100);}

void Delay::oneMillisecond()     { __delay_cycles(1000);}
void Delay::fiveMilliseconds()   { __delay_cycles(50000);}
void Delay::tenMilliseconds()    { __delay_cycles(10000);}
void Delay::hundredMilliseconds(){ __delay_cycles(100000);}
void Delay::oneSecond()          { __delay_cycles(1000000);}


void Delay::inMilliseconds(unsigned int milliseconds) {
    for (int i=milliseconds; i>0; i--) {
        __delay_cycles(1000);
    }
}

