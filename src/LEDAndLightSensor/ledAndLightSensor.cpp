
#include "ledAndLightSensor.h"
#include "../driverParameters.h"

// DriverLib
#include <gpio.h>

// App
#include <board.h>





unsigned int LEDAndLightSensor::measureLight() {
    // assert state Off

    unsigned int result;

    // Charge capacitance of LED
    toReversedFromOff();

    // No need for a delay: LED capacitance charges instantly

    // Configure p side as input
    toMeasuringFromReversed();

    result = measureByBleeding();

    toOffFromMeasuring();

    return result;
    // assert LED state is Off
}





bool LEDAndLightSensor::isNighttimeDark() {
    unsigned int sample;

    sample = measureLight();

    /*
     * Greater value is dark.
     * Discharge is through the LED as solar cell generated current.
     * That current is greater in illumination, and discharges quickly, in fewer cycles of loop.
     */
    // OLD return (sample >= DriverConstant::MinItersInLightToDischargeLEDCapacitance );
    return (sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance );
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
unsigned int LEDAndLightSensor::measureByBleeding() {
    unsigned int result;
    unsigned char value;

    // 30000 is a wildly safe value to use
    //for ( result = 0; result < DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result++) {
    for ( result = DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result > 0; result--) {

        // If LED as input is low, break (i.e. charge is dissipated.)
#ifdef UNROLL_LIGHT_SENSE_LOOP
        // hardcoded, no function call
        // This gives more resolution in the result (higher numbers)
        if (not (P1IN & BIT7)) break;
#else

        value = GPIO_getInputPinValue(NSideLEDPort, NSideLEDPin);
        /// assert value is 0 or 1
        if (  value == GPIO_INPUT_PIN_LOW )
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






/*
 * State transitions.
 * Some are public
 * States are: Off, On, ReverseBiased, Measuring
 */

void LEDAndLightSensor::toOffFromUnconfigured() {
    // Set both sides low before configuring direction out
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);

    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(PSideLEDPort,    PSideLEDPin);
    // assert N,P output
    // assert N,P low
}


void LEDAndLightSensor::toOnFromOff() {
    // assert N,P are out
    // assert N,P are low
    GPIO_setOutputHighOnPin(PSideLEDPort,    PSideLEDPin);
    // assert N low, P high
}


void LEDAndLightSensor::toOffFromOn() {
    // assert N,P are out
    // assert N low, P high
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);
    // assert N,P low
}


void LEDAndLightSensor::toOffFromMeasuring() {
    // assert measuring:
    // N is high but dir is input
    // P low
    // P output
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    // assert N,P low
    // assert N,P output
}


void LEDAndLightSensor::toReversedFromOff() {
    // assert N,P output
    // assert N,P low
    GPIO_setOutputHighOnPin(NSideLEDPort,    NSideLEDPin);
    // assert N high, P low (reverse biased)
}


void LEDAndLightSensor::toMeasuringFromReversed() {
    // assert P,N outputs
    // assert P low, N high (reversed)

    // !!! Note we don't switch N to low which would start bleeding it

    // More or less equivalent to hardcoded:   P1DIR &= ~BIT7;
    GPIO_setAsInputPin(NSideLEDPort,    NSideLEDPin);

    // assert P output
    // assert N input
    // assert P low (N value is also high, but it is an input)
}

