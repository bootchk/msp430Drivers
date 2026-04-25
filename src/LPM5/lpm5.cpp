#include "lpm5.h"

#include "../PMM/powerMgtModule.h"
#include "../SoC/SoC.h"
#include "../timer/counter.h"
#include "../clock/veryLowOscillator.h"

// temp
#include <cs.h>



/*
LPM5 class abstracts away details of LPMx.5

Here the details are:
    use internal RTC device to wake us up,
    enter LPM3.5
*/

/*
Enter a LPM

Does not return.  Instead, a reset.
Depends on a proper main() to handle the reset.
*/
void 
LPM5::haltWithRTCWakeup() 
{
    /* Require some device or pin is configured to generate interrupt to wake us.
    Here, require the RTC was configured to flag an interrupt.
    Below we also enable general interrupt, the GIE.
    */

    /* Some documents say this is required to enter LPM5.
    CS_disableClockRequest(CS_ACLK);
    */

    /* Turn off the power supply to CPU core, but not the LPM3.5 power domain
    which includes the RTC device and the VLO
    */
    PMM::configureOff();
    /* CPU power is off. 
    But there is enough energy to execute a few more instructions.
    */

    // Since power is off, this will enter LPM3.5
    SoC::enterLPM3orLPM3_5();
    // Assert GIE is on: interrupt from RTC will wakeup.
    
    // Never returns.  Continuation is a reset, i.e. to main.
}


/*
This hides that the wakeup interrupt is RTC/Counter
*/
void
LPM5::enableWakeupInterrupt()
{
    // Enable interrupt from RTC/Counter
    Counter::enableAndClearOverflowInterrupt();
}

void
LPM5::disableWakeupInterrupt()
{
    Counter::disableAndClearOverflowInterrupt();
}


/*
Configure the RTC device to wake us up.

This is a separate function
because it is called in separate cases of reset.
The RTC module must be configured the same way
whether a coldstart or LPMx.5 wakeup.

!!! Does not enable interrupt,
because enabling must wait until after LPM5 is unlocked
for a LPMx.5 wakeup.
*/
void 
LPM5::configureRTCToWakeup()
{
    // TODO ticks param

    // VLO should start when device requests it, but ensure
    VeryLowOscillator::ensureOn();

    Counter::initCountInSeconds(4);

    Counter::start();
    // assert RTC device started counting

    Counter::enableAndClearOverflowInterrupt();
    // Counter will raise flag.  Requires GIE to enable interrupt.
}

void
LPM5::unlockConfiguration()
{
    PMM::unlockLPM5();
}
