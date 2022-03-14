
#include "../src/timer/timer.h"
#include "../src/pinFunction/allPins.h"
#include "../src/delay/delay.h"

// DriverLib
#include <pmm.h>


void testTimer()
{
    // assert WDT stopped by main

    AllPins::setLowOutput();
    // Launchpad LED's off

    PMM_unlockLPM5();

    while (true)
    {

        Delay::oneSecond();
        /*
         * Expect half second duration power step while in active mode
         */

        LowPowerTimer::delaySeconds(1);
        /*
         * Expect 1 second lower power
         */

        Delay::oneSecond();

        LowPowerTimer::delayTenMilliSeconds();
        /*
         * Expect 2 millisecond lower power
         * ??? Might not be visible in EnergyTrace, too high a frequency?
         */
    }
}
