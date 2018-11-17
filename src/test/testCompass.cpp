
#include "../compass/compass.h"
#include "../pinFunction/spiPins.h"

// DriverLib
#include <pmm.h>


void testCompass()
{
    SPIPins::configure();

    PMM_unlockLPM5();

    Compass::turnPowerOn();

    Compass
    ::reset();

    // forever, single step forward
    while (true)
    {
        Compass
        ::readSingleCompassHeading();

        __delay_cycles(500000);
    }
}
