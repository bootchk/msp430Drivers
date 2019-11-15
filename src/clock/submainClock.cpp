
#include "submainClock.h"

// Implementation uses Driverlib
#include <cs.h>


void SubmainClock::init() {

    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(
                CS_SMCLK,
                CS_DCOCLKDIV_SELECT,
                CS_CLOCK_DIVIDER_1
                );

}

void SubmainClock::turnOn() {
    CS_turnOnSMCLK();
}
