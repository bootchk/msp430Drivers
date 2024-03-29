#include "../src/alarmClock/alarm/alarm.h"

#include "../src/pinFunction/allPins.h"

#include "../src/assert/myAssert.h"

// DriverLib
#include <pmm.h>




/*
 * Test: multiple alarms with LPM3 in between.
 * Here, we reconfigure Alarm after every sleep.
 *
 * Use EnergyTrace and expect one energy pulse every 10 seconds of sleep.
 *
 * !!! The code below won't set a second alarm unless the ISR is modified to exit LPM.
 */


void testAlarm2()
{
    AllPins::setHighOutput();

    PMM_unlockLPM5();

    // RTC chip is past its reset delay
    // TODO suspect since Alarm pins not configured
    myAssert (Alarm::isRTCReady() );

    while (true)
    {
        // TODO this is broken
        // WAS Alarm::configureForAlarming();
        Alarm::configureAfterColdReset();

        // myAssert ready for setAlarm()
        // myAssert serial bus (Bridge) ready, was configured

        Alarm::clearBothSidesOfSignal();

        const Duration duration = {10};

        Alarm ::setAlarmDurationSecondsFromNow(duration);

        /*
         * To test that the alarm causes an interrupt, sleep.
         * You cannot poll the alarm, because the ISR clears the interrupt.
         * In normal use, the interrupt wakes from sleep.
         */

        _low_power_mode_3();
        __no_operation();

        // The alarm ISR clears MCU side
        myAssert (Alarm::isClearOnMCUSide());

        // Test whether delay affects crash on clearAlarm
        //__delay_cycles(1000);
    }
}
