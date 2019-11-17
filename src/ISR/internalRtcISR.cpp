
#include <msp430.h> // RTC_VECTOR

#include "../timer/counter.h"

#include "../driverConfig.h"



/*
 * ISR to support Counter (and Timer) in msp430Drivers.
 */




#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
#ifdef LOW_POWER_TIMER_USE_RTC
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
#else
    // trap
    while(true) ;
#endif
}



