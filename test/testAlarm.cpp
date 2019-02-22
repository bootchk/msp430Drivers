#include "../src/alarm/alarm.h"

#include "../src/pinFunction/allPins.h"

// DriverLib
#include <pmm.h>

#include <cassert>


/*
 * Test one alarm goes off.
 *
 * Use EnergyTrace and expect one energy pulse after 10 seconds of sleep.
 * !!! The code below won't set a second alarm unless the ISR is modified to exit LPM.
 */


void testAlarm()
{
    AllPins::setHighOutput();

    PMM_unlockLPM5();

    Alarm::configureForAlarming();
    // assert ready for setAlarm()
    // assert serial bus (Bridge) ready, was configured

    Alarm::clearAlarm();

    Alarm::clearAlarm();

    assert (not Alarm::isAlarmInterruptSignalHigh() );

    while (true)
    {
        Alarm
        ::setAlarmDurationSecondsFromNow(10);

        /*
         * To test that the alarm causes an interrupt, sleep.
         * You cannot poll the alarm, because the ISR clears the interrupt.
         * In normal use, the interrupt wakes from sleep.
         */

        _low_power_mode_3();
        __no_operation();

        /*
         * ISR already cleared alarm interrupt on mcu.
         * Clear it on RTC.
         */
        Alarm::clearAlarm();
    }
}
