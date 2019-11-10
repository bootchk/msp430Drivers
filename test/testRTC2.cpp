
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

#include "../src/i2c/i2cPeripheral.h"



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
void testRTC2()
{
    PMM_unlockLPM5();

    Bridge::configureMcuSide(false);
    // assert serial bus ready

    unsigned int interruptFlags = I2CPeripheral::getInterruptFlags();

    // Test sanity
    // Registers are readable even before RTC is configured
    for (int i = 1000; i > 0; i--) {
        // test reading ID
        bool foo = RTC::isReadable();
        myAssert(foo);

        // OUT bit should be set
        myAssert(RTC::readOUTBit());
     }



    interruptFlags = I2CPeripheral::getInterruptFlags();

    // Test reading the time
    // Clock should be ticking even without any configuration
    EpochTime now;
    now = RTC::timeNowOrReset();
    // Expect now is a small integer (seconds since started testing.)

    // Can set alarm regardless configured for alarming
    RTC::setAlarmTime(1111111);

#ifdef TEMP
    // Fails?
    for (int i = 1000; i > 0; i--) {
        Bridge::writeByteWriteOnly(0x18, 0x4);

        unsigned char value = Bridge::readByte(0x18);
        myAssert(value == 0x4);
    }
#endif

    // Configure what is necessary for alarm flag set
    RTC::configureAlarmMatchPerYear();
    RTC::configure24HourMode();

    LED::configureLED1();

    // test alarming
    while (true)
    {
        delayHalfSecond();

        LED::toggle();

        now = RTC::timeNowOrReset();

        RTC::setAlarmDuration(3);
        // wait for alarm flag on RTC.  No interrupt to MCU
        while ( RTC::isAlarmFlagClear() ) ;
        RTC::clearAlarmFlag();
    }
}
