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
 * Use EnergyTrace and expect one energy pulse every x seconds of sleep.
 * !!! The code below won't set a second alarm unless the ISR is modified to exit LPM.
 *
 * This seemed like it was failing before I revamped the I2C code (when I was using the stateMachine I2C implementation.)
 * After I switched to the DriverLib implementation of I2C, it seems to work for one wakeup
 * TODO keep waking up????
 */


/*
 * !!! Differs from testAlarm:   configure RTC in every loop
 */


static void delayHalfSecond() {  __delay_cycles(500000);  }


static const Duration duration = {3};


static void configureAlarm() {
        // WAS  Alarm::configureForAlarming();
        Alarm::configureAfterColdReset();   // Configure alarm pin, bus, and RTC


        myAssert(Alarm::isRTCReadable());
        // OLD interrupt not enabled until setAlarm.    myAssert(RTC::isAlarmInterruptEnabled());

        // assert ready for setAlarm()
        // assert serial bus (Bridge) ready, was configured

        Alarm::clearBothSidesOfSignal();


        myAssert (Alarm::isClearOnMCUSide());
        myAssert (Alarm::isRTCReady() );
        // OLD interrupt not enabled until setAlarm.    myAssert(RTC::isAlarmInterruptEnabled());
}




void testAlarm3()
{
    //CentralSystem::configureAfterReset();
    SoC::enableBSLOffAndVacantMemoryNMI();
    SoC::disableFRAMWriteProtect();

    // !!! Wait for RTC to come alive
    delayHalfSecond();
    delayHalfSecond();

    //AllPins::setHighOutput();

    PMM_unlockLPM5();

    // Should only need to configure alarm once
    configureAlarm();

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
        _low_power_mode_3();
        //_low_power_mode_0();
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
