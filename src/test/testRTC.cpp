
#include "../realTimeClock/realTimeClock.h"

//#include "../pinFunction/spiPins.h"
#include "../bridge/bridge.h"

// DriverLib
#include <pmm.h>


void testRTC()
{
    PMM_unlockLPM5();

    // TODO this should be power manager
    // Compass::turnPowerOn();
    // Compass is not ready until later, require a delay

    Bridge::configureMcuSide(false);
    // assert SPI bus ready

    bool foo = RTC::isReadable();

    // forever, single step forward
    while (true)
    {
        // RTC::foo();

        __delay_cycles(500000);
    }
}
