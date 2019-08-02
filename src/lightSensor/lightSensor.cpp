
#include "lightSensor.h"

#include "../LEDAndLightSensor/ledAndLightSensor.h"






/*
 * Choose a implementation
 */
bool LightSensor::isDark() {
    // return isDarkFromLEDLightSensorUsingADC();
    return LEDAndLightSensor::isNightDark();
}

bool LightSensor::isLight() {
    return not isDark();
}


void LightSensor::calibrateInLightOrReset() {
    LEDAndLightSensor::calibrateInLightOrReset();
}
