
// DriverLib
#include <pmm.h>

#include "../src/LEDAndLightSensor/ledAndLightSensor.h"



/*
 * Test LED and light sensor will blink.
 *
 * When light is sensed, LED will blink every 10 seconds.
 * When you darken LED with your hand, it will stop blinking.
 */

void lightLED() {
    LEDAndLightSensor::toOnFromOff();
    __delay_cycles(10000);
     LEDAndLightSensor::toOffFromOn();
}

void testLEDSensor2()
{
    PMM_unlockLPM5();

    // Configure
    LEDAndLightSensor::toOffFromUnconfigured();

    // Verify LED will light up
    lightLED();

    while (true)
    {
        // One second
        __delay_cycles(1000000);

        // purpose of test
        if (not LEDAndLightSensor::isNighttimeDark() ) {
            // lightLED();
        }
    }
}
