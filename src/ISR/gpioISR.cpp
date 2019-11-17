#include <msp430.h>

#include "../LEDAndLightSensor/ledAndLightSensor.h"

#include <board.h>




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

/*
 * Check that port used by LEDAndLightSensor is port 1 OR 2
 */
#if NSideLEDPort == 1
#elif NSideLEDPort == 2
#else
#error "No port defined for LEDAndLightSensor"
#endif

/*
 * Define vectors for both ports.  Note port 3 usually has no interrupt capability.
 */

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    /*
     * Not really necessary, since _low_power_mode_off_on_exit() will clear GIE
     */
#if NSideLEDPort == 1

    LEDAndLightSensor::clearLEDNPinInterruptFlag();
    // Assert we were sleeping.  Continue after the call to _low_power_mode_x();
    _low_power_mode_off_on_exit();
#else
    // Trap interrupt for which no handler
    while (true) ;
#endif


    // For tests of alarm using external RTC (usually P1.1)
    // In some uses of LPM4.5, the ISR is not called because the alarm pin is not configured as interrupt before unlock LPM5
    // AlarmPin::clearInterrupt();
}



#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    /*
     * Not really necessary, since _low_power_mode_off_on_exit() will clear GIE
     */
#if NSideLEDPort == 2

    LEDAndLightSensor::clearLEDNPinInterruptFlag();
    // Assert we were sleeping.  Continue after the call to _low_power_mode_x();
    _low_power_mode_off_on_exit();
#else
    // Trap interrupt for which no handler
    while (true) ;
#endif
}

