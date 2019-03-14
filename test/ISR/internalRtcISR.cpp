
#include <msp430.h>

// DriverLib
#include <rtc.h>


/*
 * ISR to support Timer (msp430Drivers)
 */


#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    /*
     * RTC interrupts for many reasons.
     * We expect overflow reason, and clear only it.
     * Any other reason would loop forever.
     * Alternatively, repetitively read RTCIV to clear all flags.
     */
    RTC_clearInterrupt(RTC_BASE, RTC_OVERFLOW_INTERRUPT_FLAG);

    /*
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();
}
