
#include "../src/realTimeClock/realTimeClock.h"

#include "../src/timer/timer.h"
#include "../src/pinFunction/allPins.h"

// DriverLib
#include <pmm.h>


void testTimer()
{
    // assert WDT stopped by main

    AllPins::setHighOutput();

    PMM_unlockLPM5();

    while (true)
    {

        __delay_cycles(500000);
        /*
         * Expect half second duration power step while in active mode
         */

        LowPowerTimer::delaySeconds(2);
        /*
         * Expect 2 second lower power
         */
    }
}
