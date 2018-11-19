
#include "../compass/compass.h"

//#include "../pinFunction/spiPins.h"
#include "../bridge/bridge.h"

// DriverLib
#include <pmm.h>


void testCompass()
{
    PMM_unlockLPM5();

    // TODO this should be power manager
    // Compass::turnPowerOn();
    // Compass is not ready until later, require a delay

    Bridge::configureMcuSide();
    // assert SPI bus ready

    bool foo = Compass::isSane();

    Compass::reset();


    // forever, single step forward
    while (true)
    {
        Compass
        ::readSingleCompassHeading();

        __delay_cycles(500000);
    }
}
