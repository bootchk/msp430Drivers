
#include "veryLowOscillator.h"

// Implementation uses Driverlib
#include <cs.h>


void VeryLowOscillator::start() {
    CS_initClockSignal(
             CS_FLLREF,
             CS_VLOCLK_SELECT  ,
             CS_CLOCK_DIVIDER_1);
}

void VeryLowOscillator::allowOff() {
    // Let VLO stop when RTC stops using it
        CS_enableVLOAutoOff();
}
