
#include "veryLowOscillator.h"

// Implementation uses Driverlib
#include <cs.h>

#ifdef NOTUSED
void VeryLowOscillator::start() {
    /*
     * Nov 15, 2019 This is suspect.
     * This is saying: init the FLLREF signal to have the VLO source.
     * But what is it really doing?
     *
     * VLOCLK has no divider on FR2433??
     */
#ifdef __MSP430FR2433__
    CS_initClockSignal(
             CS_FLLREF,
             CS_VLOCLK_SELECT  ,
             CS_CLOCK_DIVIDER_1);
#else
    // FR6989 signal is always connected to oscillator
#endif
}
#endif

// Ensure the VLO is on regardless of peripheral requests.
// The opposite is allowOff.
void VeryLowOscillator::ensureOn() {
    CS_disableVLOAutoOff();
}

void VeryLowOscillator::allowOff() {
#ifdef __MSP430FR2433__
    // Let VLO stop when RTC stops using it
        CS_enableVLOAutoOff();
#else
    //  ??? not necessary
    // FUTURE FR6989
    assert(false);
#endif
}
