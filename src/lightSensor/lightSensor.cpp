
#include "lightSensor.h"

#include "../ADC/adc.h"
#include "../LEDAndLightSensor/ledAndLightSensor.h"




namespace {

// These are unreliable, determined through experimentation.
// Evidently LED as solar cell is high impedance, ADC is low impedance

unsigned int measureLightFromLEDAsSolarCellUsingADC() {
    unsigned int result;

    // Measure light using LED on P1.0
   ADC::configureForExternalPinVoltageProportionToVcc();

   result = ADC::readExternalPinVoltageProportionToVcc();
   ADC::releaseExternalPin();

   return result;
   // !!! P1.0 can't be used as LED until it is reconfigured.
}


bool isDarkFromLEDLightSensorUsingADC() {
    unsigned int lightSample = measureLightFromLEDAsSolarCellUsingADC();
    // TODO parameters.h
    return lightSample < 90;
}


bool isDarkFromLEDLightSensorUsingReverseBias() {
    return LEDAndLightSensor::isNighttimeDark();
}


}


/*
 * FUTURE: probably need to use another technique
 * where we reverse bias the LED
 * and then count time to discharge.
 *
 * Since the technique below is not reliable.
 */

/*
 * Implementation using ADC to LED as voltage generating solar cell.
 *
 * Prior implementation using ADC to solar cell was flawed,
 * since the solar cell to an analog pin powered the MCU backdoor through protection diodes of the analog pin,
 * when the MCU was supposed to be unpowered,
 * thereby draining all the solar cell power indefinitely.
 */
bool LightSensor::isDark() {
    // return isDarkFromLEDLightSensorUsingADC();
    return isDarkFromLEDLightSensorUsingReverseBias();
}



#ifdef OBSOLETE
/* Vcc varies.
 * Min Vcc is 1.9V else would not be booted.
 * Max Vcc is 3.6V.
 * Vcc in range [1.9, 3.6]
 *
 * adcResult is a proportion in range [0, 255 (0xFF)] of a reference voltage
 *
 * Return if solar cell voltage is 1/2 (0x80, 128 base 10) of Vcc i.e. about 0.9V to 1.8V
 * Return if solar cell voltage is 1/3 (0x50, 80 base 10) of Vcc i.e. about 0.6V to 1.2V
 * Return if solar cell voltage is 1/2 (0x80, 127 base 10) of 1.5V bandgap i.e. about .75V
 *
 */
bool PowerMgr::isSolarCellDark() {
    // Does not require GPIO configuration
    // This configures a GPIO as side effect.

    // Wrong: Less than 0.8 volts
    /// return ADC::measureExternalPinCentiVolts() < Parameters::MaxVccForDark  ;

    // Inaccurate: A third of Vcc
    /// bool result = ADC::measureExternalPinProportionToVcc() < 80;

    bool result = ADC::measureExternalPinProportionTo1_5VBG() < Parameters::MaxVscProportionTo1_5ForDark;
    return result;
}
#endif
