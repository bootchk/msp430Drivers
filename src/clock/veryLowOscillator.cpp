
#include "veryLowOscillator.h"

// Implementation uses Driverlib
#include <cs.h>


void VeryLowOscillator::start() {
#ifdef __MSP430FR2433__
    CS_initClockSignal(
             CS_FLLREF,
             CS_VLOCLK_SELECT  ,
             CS_CLOCK_DIVIDER_1);
#else
    // FR6989 signal is always connected to oscillator
#endif
}

void VeryLowOscillator::allowOff() {
#ifdef __MSP430FR2433__
    // Let VLO stop when RTC stops using it
        CS_enableVLOAutoOff();
#else
    // FR6989 ???
    // TODO
#endif
}
