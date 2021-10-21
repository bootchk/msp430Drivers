


/*
 * Test a UV led as a light sensor.
 *
 * You should not use the led as a light source without a limit resistor:
 * - it is UV, won't be visible
 * - without a limit resistor the current is unlimited and could damage the led
 *
 * Sensor is between pins TODO
 */



// DriverLib
#include <pmm.h>

#include "../src/SoC/SoC.h"

#include "../src/LEDAndLightSensor/ledAndLightSensor.h"
#include "../src/LED/led.h"

#include "../src/assert/myAssert.h"



/*
 * Test UV light sensor. Toggle red led every half second
 *
 * Every half second, sense UV light.
 * When light is sensed, green LED will be on.
 *
 * When you darken LED with your hand, green led will turn off.
 */



unsigned long sample;

unsigned int samples[10];
unsigned int index = 0;


void delayHalfSecond() {
    __delay_cycles(500000);

}


void testLEDSensor3()
{
    // !!!
    SoC::disableFRAMWriteProtect();


    PMM_unlockLPM5();

    // Configure sensor to off
    LEDAndLightSensor::toOffFromUnconfigured();



    LED::configureLED1();   // red
    LED::configureLED2();   // green

    // Require this test start in light conditions.
    // TODO for LIGHT_SENSE_ITERATIVE
    // LEDAndLightSensor::calibrateInLightOrReset();

    // If sensor indicates dark, sensor must be calibrated wrong.
    // myAssert(not LEDAndLightSensor::isNightDark());


    /*
     * Dynamic test:
     * Leave LED in UV light, expect green led lit.
     * Darken LED with your hand, expect green LED to turn off.
     */
    while (true)
    {
        delayHalfSecond();

        LED::toggleLED1();  // red

        sample = LEDAndLightSensor::measureLight();

        // purpose of test

        // isNightDark samples again, and uses a defined constant
        // if (LEDAndLightSensor::isNightDark() ) {

        // 64k @1_2kHz is about 50 seconds
        if (sample < 1800) {
            LED::turnOnLED2();
        }
        else {
            LED::turnOffLED2();
        }

        samples[index] = sample;
        index++;
        if (index > 9) index = 0;
    }
}
