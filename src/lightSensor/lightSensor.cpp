
#include "lightSensor.h"

#include "../LEDAndLightSensor/ledAndLightSensor.h"






/*
 * Choose a implementation
 */
bool LightSensor::isDark() {
    // return isDarkFromLEDLightSensorUsingADC();
    return LEDAndLightSensor::isNighttimeDark();
}
