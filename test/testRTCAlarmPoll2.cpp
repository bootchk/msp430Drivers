
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

#include "../src/i2c/i2cPeripheral.h"
#include "../src/busConfig.h"



// DriverLib
#include <pmm.h>


static void delayHalfSecond() {
    __delay_cycles(500000); // half second
}


static void configureAlarm() {
    // Configure what is necessary for alarm flag set
    RTC::configureStandardAlarming();
}

/*
 * Test external RTC driver.
 *
 * In a loop, set alarm 3 seconds from now, poll for alarm, and toggle LED on Launchpad.
 * Expect: LED to blink forever.
 *
 * !!! Here we reconfigure alarm in the loop
 *
 * Exception: if something hangs, LED will stop blinking.
 * Driver not now designed to timeout if the MCU EUSCIB peripheral hangs.
 *
 * Exception: RTC is dead.
 * Driver will return an invalid time and softfault, which ???
 */
void testRTCAlarmPoll2()
{
    static unsigned int countAlarms = 0;
    static unsigned int countPolling = 0;

    PMM_unlockLPM5();

    Bridge::configureMcuSide(RTCBusAddress, false);
    // assert serial bus ready

    // Test sanity
    myAssert(RTC::isReadable());

    LED::configureLED1();

    // test alarming
    while (true)
    {
        delayHalfSecond();

        LED::toggle();

        RTC::clearAlarmFlag();

        configureAlarm();

        RTC::setAlarmDuration(3);

        myAssert(RTC::isAlarmFlaggingConfigured());

        // wait for alarm flag on RTC.  No interrupt to MCU
        countPolling = 0;
        while ( RTC::isAlarmFlagClear() ) countPolling++;

        countAlarms++;

    }
}
