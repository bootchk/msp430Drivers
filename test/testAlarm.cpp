#include "../src/alarmClock/alarm/alarm.h"
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

#include "../src/pinFunction/allPins.h"
#include "../src/pinFunction/alarmPin.h"
#include "../src/SoC/SoC.h"

#include "../src/assert/myAssert.h"

// DriverLib
#include <pmm.h>




/*
 * Test alarms go off forever.
 *
 * Use EnergyTrace and expect one energy pulse every 10 seconds of sleep.
 * !!! The code below won't set a second alarm unless the ISR is modified to exit LPM.
 *
 * This seemed like it was failing before I revamped the I2C code (when I was using the stateMachine I2C implementation.)
 * After I switched to the DriverLib implementation of I2C, it seems to work for one wakeup
 * TODO keep waking up????
 */


static const Duration duration = {3};


void testAlarm()
{
    //CentralSystem::configureAfterReset();
    SoC::enableBSLOffAndVacantMemoryNMI();
    SoC::disableFRAMWriteProtect();

    //AllPins::setHighOutput();

    PMM_unlockLPM5();

    // WAS  Alarm::configureForAlarming();
    Alarm::configureAfterColdReset();   // Configure alarm pin, bus, and RTC


    myAssert(Alarm::isRTCReadable());
    // OLD not enabled until setAlarm     myAssert(RTC::isAlarmInterruptEnabled());

    // assert ready for setAlarm()
    // assert serial bus (Bridge) ready, was configured

    Alarm::clearBothSidesOfSignal();

    Alarm::clearBothSidesOfSignal();

    myAssert (Alarm::isClearOnMCUSide());

    myAssert (Alarm::isRTCReady() );

    // OLD not enabled until setAlarm    myAssert(RTC::isAlarmInterruptEnabled());

    while (true)
    {
        bool didSet = Alarm::setAlarmDurationSecondsFromNow(duration);
        myAssert(didSet);

        // alarm not already off
        myAssert (Alarm::isClearOnMCUSide());

        // alarm interrupt enabled on MCU side
        myAssert(AlarmPin::isEnabledInterrupt());

        // alarm completely configured on RTC side
        myAssert(RTC::isAlarmInterruptConfigured());



        /*
         * To test that the alarm causes an interrupt, sleep.
         * You cannot poll the alarm, because the ISR clears the interrupt.
         * In normal use, the interrupt wakes from sleep.
         */

        // One alternative for waiting.
        // Wait for RTC to pull alarm pin low.  Interrupt will occur on rising edge.
        // while (not Alarm::isRTCReady());

        // Other alternatives for waiting.
        // _low_power_mode_3();
        _low_power_mode_0();
        __no_operation();

        // Test whether delay affects crash on clearAlarm
        //__delay_cycles(1000);

        /*
         * Test that the alarm pin is high.
         * The interrupt may occur so fast (on the rising edge)
         * that the pin might not be completely high when we test here???
         */
        //myAssert (Alarm::isRTCReady() );

        /*
         * ISR already cleared alarm interrupt on mcu.
         * Clear it on RTC.
         */
        Alarm::clearBothSidesOfSignal();
    }
}
