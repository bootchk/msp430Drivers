
#include "ledAndLightSensor.h"
#include "../config/driverConfig.h"
#include "../config/driverParameters.h"

#include "../softFault/softFault.h"


namespace {

/*
 * The calibrated count that the light sensor will return if it is dark.
 */
#pragma PERSISTENT
unsigned int referenceLightSensorDarkCount = 0;

#ifdef LIGHT_SENSE_SLEEPING
bool isSampleIndicateLight(unsigned int sample) {
    return (sample < DriverConstant::TicksInNightDarkToDischargeLEDCapacitance);
}

bool isSampleIndicateNightDark(unsigned int sample) {
    return not isSampleIndicateLight(sample);
}
#endif
// TODO ifdef LIGHT_SENSE_ITERATIVE use different parameters




unsigned int sampleInLightOrReset() {
    unsigned int sample;

        sample = LEDAndLightSensor::measureLight();

        if ( isSampleIndicateNightDark(sample) ) {

            // OLD sample >= DriverConstant::MaxItersInDarkToDischargeLEDCapacitance) {
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




unsigned long LEDAndLightSensor::measureLight() {
    // assert state Off

    unsigned long result;

    // Charge capacitance of LED
    toReversedFromOff();

    // No need for a delay: LED capacitance charges instantly

    // Configure p side as input
    toMeasuringFromReversed();

    // Choice of implementation
#ifdef LIGHT_SENSE_ITERATIVE
    result = measureCapacitanceDischargeIteratively();
#else
    result = measureCapacitanceDischargeSleeping();
#endif

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

#ifdef LIGHT_SENSE_ITERATIVE
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
#endif

bool LEDAndLightSensor::isNightDark() {
    unsigned int sample;

    sample = measureLight();

    return isSampleIndicateNightDark(sample);
}





#ifdef LIGHT_SENSE_ITERATIVE

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
#endif
