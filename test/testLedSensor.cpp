#include "../src/alarm/alarm.h"

// DriverLib
#include <pmm.h>

#include "../src/LEDAndLightSensor/ledAndLightSensor.h"

#include <cassert>


/*
 * Test LED and light sensor will blink.
 *
 * When light is sensed, LED will blink every 10 seconds.
 * When you darken LED with your hand, it will stop blinking.
 */


void testLEDSensor()
{
    PMM_unlockLPM5();

    Alarm::configureForAlarming();
    // assert ready for setAlarm()
    // assert serial bus (Bridge) ready, was configured

    Alarm::clearBothSidesOfSignal();

    assert (Alarm::isRTCReady() );

    // Configure
    LEDAndLightSensor::toOffFromUnconfigured();

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

        /*
         * ISR already cleared alarm interrupt on mcu.
         * Clear it on RTC.
         */
        Alarm::clearBothSidesOfSignal();


        // purpose of test
        if (not LEDAndLightSensor::isNighttimeDark() ) {
            LEDAndLightSensor::toOnFromOff();
            __delay_cycles(10000);
            LEDAndLightSensor::toOffFromOn();

        }
    }
}
