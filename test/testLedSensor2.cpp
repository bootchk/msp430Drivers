
// DriverLib
#include <pmm.h>

#include "../src/SoC/SoC.h"

#include "../src/LEDAndLightSensor/ledAndLightSensor.h"

#include "../src/assert/myAssert.h"



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

unsigned int sample;


void testLEDSensor2()
{
    // !!!
    SoC::disableFRAMWriteProtect();


    PMM_unlockLPM5();

    // Configure
    LEDAndLightSensor::toOffFromUnconfigured();

    // Verify LED will light up
    lightLED();

    // Require this test start in light conditions.
    LEDAndLightSensor::calibrateInLightOrReset();

    // If sensor indicates dark, sensor must be calibrated wrong.
    myAssert(not LEDAndLightSensor::isNightDark());


    /*
     * Dynamic test:
     * Leave LED in light, expect no LED blinking.
     * Darken LED with your hand, expect LED to blink while dark.
     */
    while (true)
    {
        // One second
        __delay_cycles(1000000);

        sample = LEDAndLightSensor::measureLight();

        // purpose of test
        if (not LEDAndLightSensor::isNightDark() ) {
            // lightLED();
        }
    }
}
