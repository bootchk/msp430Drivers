

#include "../src/timer/timer.h"
#include "../src/delay/delay.h"
#include "../src/pinFunction/allPins.h"

// DriverLib
#include <pmm.h>
#include <gpio.h>


/*
 * Timer uses internal RTC.
 *
 * Uses no pins.
 *
 * Uses ISR for internal RTC.
 * ISR exits sleep
 *
 *
 * Expect in energy trace:
 * initial current of ????
 * repeat:
 *    sleep 5 seconds, current ???
 *    current spike for interrupt
 *    spinning delay of 1 second, current .3mW (126uA @ 3V @ 1Mhz clock)
 *
 */



void testTimer2() {

    AllPins::setLowOutput();
    AllPins::configureGPIOLowPower();

    PMM_unlockLPM5();

    //PMM_disableSVSH();

    while(true) {
        LowPowerTimer::delayFiveSeconds();
        Delay::oneSecond();
    }
}
