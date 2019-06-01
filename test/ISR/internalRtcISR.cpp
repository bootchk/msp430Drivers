
#include <msp430.h> // RTC_VECTOR

// msp430Drivers
#include "../../src/timer/counter.h"


/*
 * ISR to support Counter (and Timer) in msp430Drivers.
 */


#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    Counter::setOverflowFlag();

    /*
     * RTC interrupts for many reasons.
     * We expect overflow reason, and clear only it.
     * Any other reason would loop forever.
     * Alternatively, repetitively read RTCIV to clear all flags.
     */
    Counter::disableAndClearOverflowInterrupt();

    /*
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();
}
