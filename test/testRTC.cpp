
#include "../src/realTimeClock/realTimeClock.h"

#include "../src/bridge/bridge.h"

// DriverLib
#include <pmm.h>


void testRTC()
{
    PMM_unlockLPM5();

    Bridge::configureMcuSide(false);
    // assert SPI bus ready

    // Test sanity
    // Registers are readable even before RTC is configured
    bool foo = RTC::isReadable();
    // Expect foo is true


    // Test reading the time
    // Clock should be ticking even without any configuration
    EpochTime now;
    now = RTC::timeNowOrReset();
    // Expect now is a small integer (seconds since started testing.)

    // More testing of RTC is in test of Alarm

    while (true)
    {
        __delay_cycles(500000);
    }
}
