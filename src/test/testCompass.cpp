
#include "../compass/compass.h"




void testCompass()
{
    // todo spi ready
    // TODO unlock lpm5

    Compass::turnPowerOn();

    Compass
    ::reset();

    // forever, single step forward
    while (true)
    {
        Compass
        ::readCompassHeading();

        __delay_cycles(500000);
    }
}
