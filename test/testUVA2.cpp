
/*
 * Test a GUVA-S12SD sensor on a breakout board having an opamp as a TIA to convert current to voltage.
 */

#include "../src/ADC/adc.h"

#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

#include "../src/i2c/i2cPeripheral.h"

#include "../src/config/busConfig.h"  // slave address

#include "../src/UVSensor/UVCommands.h"


// DriverLib
#include <pmm.h>


static void delayHalfSecond() {
    __delay_cycles(500000); // half second
}



/*
 * Test UVA sensor driver.
 *
 * In a loop, read UVA and toggle LED on Launchpad.
 * Expect: LED to blink forever.
 *
 * Exception: if something hangs, LED will stop blinking.
 * Driver not now designed to timeout if the MCU EUSCIB peripheral hangs.
 *
 * Exception: RTC is dead.
 * Driver will return an invalid time and softfault, which ???
 *
 * Does not test configuration of RTC.
 *
 * More testing of RTC is in test of Alarm
 */
void testUVSensor2()
{
    unsigned int centivolts;

    PMM_unlockLPM5();

    // Test sanity

    centivolts = ADC::measureVccCentiVolts();
    // Vcc is 3.3V
    myAssert( (centivolts > 300) and (centivolts < 350) );


    LED::configureLED1();
    LED::configureLED2();   // green


    ADC::configureForExternalPinVoltageProportionToVcc();

    while (true)
    {
        delayHalfSecond();

        LED::toggle();

        centivolts = ADC::measureExternalPinProportionToVcc();

        if (centivolts)  LED::turnOnLED2();
        else LED::turnOffLED2();
    }
}
