
#include <msp430.h>


/*
 * ISR to support LowPowerTimer using WDT implementation (msp430Drivers)
 *
 * !!! Two WDT interrupt vectors.
 * One is the reset vector.
 * This is for interval timer mode.
 */


#pragma vector = WDT_VECTOR
__interrupt void WDT_A_ISR(void)
{
#ifdef LOW_POWER_TIMER_USE_WDT
    /*
     * The IFG flag for the WDT is cleared automatically on return from interrupt.
     */

    /*
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();
#else
    // trap
    while (true)  ;
#endif
}
