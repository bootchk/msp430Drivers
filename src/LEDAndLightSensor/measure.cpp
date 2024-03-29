#include "ledAndLightSensor.h"
#include "../config/driverParameters.h"
#include "../timer/counter.h"
#include "../assert/myAssert.h"

#include <msp430.h> // LPM3 macro





/*
 * I found that to measure very dark (say less than 10 lux) requires > 5k iterations.
 * Which is about 50k instruction cycles, or 50ms at 1Mhz clock.
 * Measuring iteratively, cpu takes about 1mA.
 *
 * There are to implementations:
 * iterative: higher power: 600uA or 1mA (active mode) for 50ms =  30k mSec*uA
 * sleeping (lower power: 1uA (LPM3) for 0.5 seconds  =   500 mSec*uA
 *
 * To detect 10 lux
 */



/*
 * Implementation using interrupts.
 * Two processes:
 * - capacitor discharge on GPIO pin to low
 * - countdown timer
 *
 * One will finish first.
 * Sleep until one finishes.
 *
 * The result is the counter value, up to the max defined by the countdown timer.
 *
 * Requires ISR for both GPIO and Counter.
 * - GPIO ISR clear IFG bit and exit sleep.
 * - Counter ISR clear IFG bit, set overflow flag to main, and exit sleep
 */
unsigned int LEDAndLightSensor::measureCapacitanceDischargeSleeping() {
    unsigned int result;

    /*
     * Discharge of pin capacitance might already have happened.
     * So transition from high to low also might have already happened.
     * We enable interrupt on high-to-low transition.
     * If transition occurred prior and pin is low, enabling edge will set IFG.
     * Assert GIE is not enabled, so no interrupt will occur yet.
     */
    clearLEDNPinInterruptFlag();
    enableHighToLowInterruptFromLEDNPin();
    /*
     * IFG is set already if pin is already low.
     */

    /*
     * Start a counter parallel to other process (capacitor discharging.)
     *
     * @ 12kHz max duration is 64k ticks, i.e. 5 seconds
     * @ 1.5kHz max duration is 64k ticks, i.e. 42 seconds
     *
     * 5000 ticks = 0.5 second, time to discharge Cree566 in 10 lux
     */
    // Counter::init12kHz(DriverConstant::MaxTicksInDarkToDischargeLEDCapacitance);
    Counter::init1_2kHz(DriverConstant::MaxTicksInDarkToDischargeLEDCapacitance);

    Counter::start();
    // assert counter is near zero since start() clears it.

    /*
     * Enter low power until interrupt for pin low OR counter overflow.
     * Does not return until interrupt.
     * Assert ISR exits low power and clears GIE, and execution continues after this call.
     *
     * There is no race to sleep before interrupt.
     * The interrupt condition (IFG) may already exist:
     * when this enables GIE, interrupt will occur immediately without an effective sleep.
     */
    _low_power_mode_3();
    __no_operation();

    /*
     * One or both processes interrupted.
     * Counter may still be counting and may yet set IFG.
     * Pin may yet set IFG.
     * But GIE is now clear.
     */

    /*
     * If counter overflowed, result will be maxDurationInTicks
     */
    result = Counter::getCount();

    // Counter is up, will be less than or equal to maxDurationInTicks.
    myAssert(result<=DriverConstant::MaxTicksInDarkToDischargeLEDCapacitance);

    /*
     * We don't expect count is zero,
     * but it might happen if the LED is sensitive or light is bright.
     * Also, the capacitance has already started to discharge before we start the counter.
     */

    /*
     * Disable both interrupt sources.
     * One should have been already disabled by ISR.
     * Not really necessary since GIE is clear, but restore system to previous state.
     */
    disableLEDNPinInterrupt();
    Counter::stop();    // disables interrupt

    return result;
}



/*
 * Low power considerations:
 * the loop body takes tens of instructions.
 * The CPU is active during the loop.
 * We want to minimize the max loop iterations to minimize power consumed.
 *
 * We also want to minimize time spent in loop body.
 * That makes the count a larger number.
 * We can make the loop tighter by counting down.
 */
unsigned int LEDAndLightSensor::measureCapacitanceDischargeIteratively() {
    unsigned int result;


    // 30000 is a wildly safe value to use
    //for ( result = 0; result < DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result++) {
    for ( result = DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result > 0; result--) {

        // If LED as input is low, break (i.e. charge is dissipated.)
#ifdef UNROLL_LIGHT_SENSE_LOOP
        // hardcoded, no function call
        // This gives more resolution in the result (higher numbers)
        if (not (P1IN & BIT7)) break;
#else
        if (isLEDNPinLow())
            break;
#endif
    }
    // assert 0 <= result <= MaxItersInDarkToDischargeLEDCapacitance
    // assert result is larger if environment is illuminating LED

    /*
     * Return the number of iterations.
     * Since loop index is counting down, subtract
     */
    return DriverConstant::MaxItersInDarkToDischargeLEDCapacitance - result;
    // assert state still measuring

    // assert result is small number (<MaxItersInDarkToDischargeLEDCapacitance) if environment is illuminating LED as light sensor
    // else result == MaxItersInDarkToDischargeLEDCapacitance
}
