
#include <msp430.h>


/*
 * ISR to support LowPowerTimer using WDT implementation (msp430Drivers)
 *
 * !!! Two WDT interrupt vectors.
 * This is for interval timer mode
 */


#pragma vector = WDT_VECTOR
__interrupt void WDT_A_ISR(void)
{
    /*
     * There is no IFG flag for the WDT?
     */

    /*
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();
}
