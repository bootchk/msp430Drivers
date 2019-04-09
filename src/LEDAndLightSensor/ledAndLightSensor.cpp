
#include "ledAndLightSensor.h"
#include "../driverParameters.h"

#include "../assert/fatal.h"


namespace {

/*
 * The calibrated count that the light sensor will return if it is dark.
 */
#pragma PERSISTENT
unsigned int referenceLightSensorDarkCount = 0;

}




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
    // insure result in [0, MaxItersInDarkToDischargeLEDCapacitance]
}





bool LEDAndLightSensor::isNighttimeDark() {
    unsigned int sample;

    sample = measureLight();

    /*
     * Greater value is dark.
     * Discharge is through the LED as solar cell generated current.
     * That current is greater in illumination, and discharges quickly, in fewer cycles of loop.
     */

    /*
     * Compare to a defined constant less than max iterations.
     */
    // OLD return (sample >= DriverConstant::MinItersInLightToDischargeLEDCapacitance );
    /*
     * Compare to the max iterations, also a defined constant.
     */
    //OLD return (sample >= referenceLightSensorDarkCount );

    /*
     * NOW: compare to run-time calibrated value
     */
    return (sample >= referenceLightSensorDarkCount
            or sample >= referenceLightSensorDarkCount
            );
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


    // 30000 is a wildly safe value to use
    //for ( result = 0; result < DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result++) {
    for ( result = DriverConstant::MaxItersInDarkToDischargeLEDCapacitance; result > 0; result--) {

        // If LED as input is low, break (i.e. charge is dissipated.)
#ifdef UNROLL_LIGHT_SENSE_LOOP
        // hardcoded, no function call
        // This gives more resolution in the result (higher numbers)
        if (not (P1IN & BIT7)) break;
#else
        if (isLow())
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




void LEDAndLightSensor::calibrateInLightOrReset() {
    unsigned int sample;

    sample = measureLight();

    if (sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance) {
        /*
         * Seems like this was called when it is dark.
         * It took more iterations to discharge capacitance than should be.
         */
        // FUTURE not SW but operating
        Fatal::fatalSWFault();
    }
    else {
        // Save calibrated reference value
        referenceLightSensorDarkCount = sample + 50;
        // TODO magic number 50
    }

}


