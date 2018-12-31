
#include "../src/compass/compass.h"

//#include "../pinFunction/spiPins.h"
#include "../src/bridge/bridge.h"

// DriverLib
#include <pmm.h>


void testCompass()
{
    PMM_unlockLPM5();

    // TODO this should be power manager
    // Compass::turnPowerOn();
    // Compass is not ready until later, require a delay

    Bridge::configureMcuSide(true);
    // assert serial bus ready

    bool foo = Compass::isSane();

    unsigned char bar = Compass::readStatus();

    Compass::reset();


    // forever, single step forward
    while (true)
    {
        Compass
        ::readSingleCompassHeading();

        __delay_cycles(500000);
    }
}
