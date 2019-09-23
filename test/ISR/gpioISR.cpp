#include <msp430.h>

#include "../../src/LEDAndLightSensor/ledAndLightSensor.h"

/*
 * TODO Make this dependent on board.h
 * Define redundant vectors; only one is used.
 */
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
 *
 * If this is not defined correctly,
 * the symptom is a call to isr_trap.ism, a TI supplied routine for missing ISR's.
 */


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    /*
     * Not necessary, since _low_power_mode_off_on_exit() will clear GIE
     */
    LEDAndLightSensor::clearLEDNPinInterruptFlag();

    /*
     * Do nothing except exit low power mode.
     * The continuation is after the call to _low_power_mode_x();
     */
    _low_power_mode_off_on_exit();

    // not assert interrupt flag clear: if condition still exists, flag is still set
}


#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    LEDAndLightSensor::clearLEDNPinInterruptFlag();
    _low_power_mode_off_on_exit();
}
