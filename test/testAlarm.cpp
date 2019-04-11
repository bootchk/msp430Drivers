#include "../src/alarmClock/alarm/alarm.h"

#include "../src/pinFunction/allPins.h"

#include <cassert>

// DriverLib
#include <pmm.h>




/*
 * Test multiple alarms go off.
 *
 * Use EnergyTrace and expect one energy pulse every 10 seconds of sleep.
 * !!! The code below won't set a second alarm unless the ISR is modified to exit LPM.
 *
 * This fails.
 * Something about the configuration of eUSCI not surviving a LPM3.
 * Failure symptom is erratic power curver after first wake.
 */


void testAlarm()
{
    AllPins::setHighOutput();

    PMM_unlockLPM5();

    // TODO broken
    // WAS Alarm::configureForAlarming();

    // assert ready for setAlarm()
    // assert serial bus (Bridge) ready, was configured

    Alarm::clearBothSidesOfSignal();

    Alarm::clearBothSidesOfSignal();

    assert (Alarm::isClearOnMCUSide());

    assert (Alarm::isRTCReady() );

    while (true)
    {
        static const Duration duration = {10};
        Alarm::setAlarmDurationSecondsFromNow(duration);

        /*
         * To test that the alarm causes an interrupt, sleep.
         * You cannot poll the alarm, because the ISR clears the interrupt.
         * In normal use, the interrupt wakes from sleep.
         */

        _low_power_mode_3();
        __no_operation();

        // Test whether delay affects crash on clearAlarm
        //__delay_cycles(1000);

        assert (Alarm::isRTCReady() );

        /*
         * ISR already cleared alarm interrupt on mcu.
         * Clear it on RTC.
         */
        Alarm::clearBothSidesOfSignal();
    }
}
