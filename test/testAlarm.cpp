#include "../src/alarm/alarm.h"

// DriverLib
#include <pmm.h>

#include <cassert>




void testAlarm()
{
    PMM_unlockLPM5();

    Alarm::configureForAlarming();
    // assert ready for setAlarm()

    Alarm::clearAlarm();

    assert (not Alarm::isAlarmInterruptSignalHigh() );

    Alarm::setAlarmDurationSecondsFromNow(10);

    /*
     * To test that the alarm causes an interrupt, sleep.
     * You cannot poll the alarm, because the ISR clears the interrupt.
     * In normal use, the interrupt wakes from sleep.
     */

    _low_power_mode_3();
    __no_operation();
}
