
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



/*
 * Test external RTC driver.
 *
 * In a loop, read time and toggle LED on Launchpad.
 * Expect: LED to blink forever.
 *
 * Exception: if something hangs, LED will stop blinking.
 * Driver not now designed to timeout if the MCU EUSCIB peripheral hangs.
 *
 * Exception: RTC is dead.
 * Driver will return an invalid time and softfault, which ???
 *
 * Does not test configuration of RTC.
 *
 * More testing of RTC is in test of Alarm
 */
void testRTC()
{
    PMM_unlockLPM5();

    Bridge::configureMcuSide(RTCBusAddress, false);
    // assert serial bus ready

    unsigned int interruptFlags = I2CPeripheral::getInterruptFlags();

    // Test sanity
    // Registers are readable even before RTC is configured
    bool foo = RTC::isReadable();
    myAssert(foo);


    interruptFlags = I2CPeripheral::getInterruptFlags();

    // Test reading the time
    // Clock should be ticking even without any configuration
    EpochTime now;
    now = RTC::timeNowOrReset();
    // Expect now is a small integer (seconds since started testing.)

    LED::configureLED1();

    while (true)
    {
        delayHalfSecond();

        LED::toggleLED1();

        now = RTC::timeNowOrReset();
    }
}
