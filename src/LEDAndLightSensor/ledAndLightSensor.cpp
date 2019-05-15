
#include "ledAndLightSensor.h"
#include "../driverParameters.h"

#include "../softFault/softFault.h"


namespace {

/*
 * The calibrated count that the light sensor will return if it is dark.
 */
#pragma PERSISTENT
unsigned int referenceLightSensorDarkCount = 0;


unsigned int sampleInLightOrReset() {
    unsigned int sample;

        sample = LEDAndLightSensor::measureLight();

        if (sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance) {
            /*
             * Seems like this was called when it is dark.
             * It took more iterations to discharge capacitance than should be.
             * We can't proceed unless we can detect light.
             */
            SoftFault::failDetectLight();
        }
        return sample;
}

}   // namespace




unsigned int LEDAndLightSensor::measureLight() {
    // assert state Off

    unsigned int result;

    // Charge capacitance of LED
    toReversedFromOff();

    // No need for a delay: LED capacitance charges instantly

    // Configure p side as input
    toMeasuringFromReversed();

    // Choice of implementation
    //result = measureCapacitanceDischargeIteratively();
    result = measureCapacitanceDischargeSleeping();

    toOffFromMeasuring();

    return result;
    // assert LED state is Off
    // insure result in [0, MaxItersInDarkToDischargeLEDCapacitance]
}







/*
 * Sample is an int <= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance
 *
 * Greater value is dark.
 * Discharge is through the LED as solar cell generated current.
 * That current is greater in illumination, and discharges quickly, in fewer cycles of loop.

 * Compare to a defined constant less than max iterations.
 OLD return (sample >= DriverConstant::MinItersInLightToDischargeLEDCapacitance );

 * Compare to the max iterations, also a defined constant.
 OLD return (sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance );
 */

// Not used anymore?
// Measuring twilight (100 lux) is different, requires calibrated sensor.
bool LEDAndLightSensor::isTwilightDark() {
    unsigned int sample;

    sample = measureLight();
    // NOW: compare to run-time calibrated value OR the max iterations
    return (sample >= referenceLightSensorDarkCount
                or sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance
                );
}


bool LEDAndLightSensor::isNightDark() {
    unsigned int sample;

    sample = measureLight();

    return (sample >= DriverConstant::TicksInNightDarkToDischargeLEDCapacitance);
}






void LEDAndLightSensor::calibrateInLightOrReset() {

    unsigned int sampleSum = 0;

    // sum two samples
    for(int i=2; i>0; i--) {
        unsigned int sample;
        // May reset, never return
        sample = sampleInLightOrReset();
        sampleSum += sample;
    }

    // Calculate average plus factor.
    // Save calibrated reference value to persistent memory
    referenceLightSensorDarkCount = sampleSum/2 + DriverConstant::DarkFactorLEDDischargeCount;
}
