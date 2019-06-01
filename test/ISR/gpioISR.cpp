#include <msp430.h>

#include "../../src/LEDAndLightSensor/ledAndLightSensor.h"


/*
 * ISR for interrupts on GPIO.
 *
 * !!! The GPIO pin for alarm does causes interrupts,
 * but that wakes from LPM4.5,
 * and ISR is never called because of the way we exit LPM4.5 (not enabling GPIO interrupt before clearing LPM5 bit.)
 *
 * This ISR is thus for servicing ISR for other GPIO pins.
 *
 * Now: LEDAndLightSensor.
 *
 * FUTURE This is not part of embeddedDutyCycle, but part of the application.
 */


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    /*
     * Not really necessary, since _low_power_mode_off_on_exit() will clear GIE
     */
    LEDAndLightSensor::clearLEDNPinInterruptFlag();

    /*
     * We don't do anything here except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();

    // not assert interrupt flag clear: if condition still exists, flag is still set
}
