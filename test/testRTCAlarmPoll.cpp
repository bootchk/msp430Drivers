
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

#include "../src/i2c/i2cPeripheral.h"

#include "../src/config/busConfig.h"



// DriverLib
#include <pmm.h>


static void delayHalfSecond() {
    __delay_cycles(500000); // half second
}

const Duration ThreeSeconds = {3};

/*
 * Test external RTC driver.
 *
 * In a loop, set alarm 3 seconds from now, poll for alarm, and toggle LED on Launchpad.
 * Expect: LED to blink forever.
 *
 * Exception: if something hangs, LED will stop blinking.
 * Driver not now designed to timeout if the MCU EUSCIB peripheral hangs.
 *
 * Exception: RTC is dead.
 * Driver will return an invalid time and softfault, which ???
 */
void testRTCAlarmPoll()
{
    static unsigned int countAlarms = 0;
    static unsigned int countPolling = 0;

    PMM_unlockLPM5();

    Bridge::configureMcuSide(RTCBusAddress, false);
    // assert serial bus ready

    // Test sanity
    myAssert(RTC::isReadable());

    // Configure what is necessary for alarm flag set
    RTC::configureStandardAlarming();

    LED::configureLED1();

    // test alarming
    while (true)
    {
        delayHalfSecond();

        LED::toggleLED1();

        RTC::clearAlarmFlag();

        RTC::setAlarmDuration(ThreeSeconds);

        myAssert(RTC::isAlarmFlaggingConfigured());

        // wait for alarm flag on RTC.  No interrupt to MCU
        countPolling = 0;
        while ( RTC::isAlarmFlagClear() ) countPolling++;

        countAlarms++;

    }
}
