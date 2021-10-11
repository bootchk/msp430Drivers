
#include "delay.h"

// DriverLib
#include <msp430.h>     // __delay_cycles



/*
 * Only for 1Mhz cpu clock speed.
 */


void
Delay::oneMillisecond() {
    __delay_cycles(1000);
}


void
Delay::fiveMilliseconds() {
    __delay_cycles(50000);
}

void
Delay::tenMilliseconds() {
    __delay_cycles(10000);
}

void
Delay::oneSecond() {
    __delay_cycles(1000000);
}


