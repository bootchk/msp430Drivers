
#include "../compass/compass.h"




void testCompass()
{
    Compass
    ::reset();
    // assert GPIO configured

    // TODO unlock lpm5

    Compass::turnPowerOn();

    // forever, single step forward
    while (true)
    {
        Compass
        ::readCompassHeading();

        __delay_cycles(500000);
    }
}
