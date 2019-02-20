
#include "adc.h"
#include "ADCConfigure.h"


// DriverLib
#include <adc.h>



/*
 * See mspware/examples/device/.../msp430fr243x_adc10_05.c
 * for example code to measure vcc
 * from which I derived this code.
 */

#ifdef OLD
void ADC::configureExternalPinVoltagePin() {
    GPIO_setAsPeripheralModuleFunctionInputPin(
            ExternalPinVoltagePort,
            ExternalPinVoltagePin,
            ExternalPinVoltagePinFunction);
}
#endif


// For 8-bit resolution
#define MaxADCRead 255





unsigned int ADC::measureVccCentiVolts() {
    ADCConfigure::configureForVccMeasure();

    // Assert VBG internal bandgap reference is ready

    unsigned int adcResult = read();

    ADCConfigure::unconfigureForVccMeasure();

    return convert8BitADCReadToCentiVolts(adcResult);
}


/*
 * Compare voltage on solar cell to Vcc
 *
 */
unsigned int ADC::measureExternalPinProportionToVcc() {
    ADCConfigure::configureForExternalPinVoltageProportionToVcc();

    // !!!! No delay from prior configuration/use of pin, for signal to stabililize?

    unsigned int adcResult = read();

    // assert result in range [0,255]
    return adcResult;
}



unsigned int ADC::measureExternalPinProportionTo1_5VBG() {
    ADCConfigure::configureForExternalPinVoltageProportionTo1_5VBG();

    unsigned int adcResult = read();

    // assert result in range [0,255]
    return adcResult;
}




#ifdef WRONG


unsigned long ADC::measureExternalPinCentiVolts() {
    configureForExternalPinVoltagePin();

    unsigned int adcResult = read();

    return convertADCReadToCentiVolts(adcResult);
}
#endif


/*
 * Private read routine
 */
unsigned int ADC::read()
{
    // Require configuration

    //Enable and Start conversion  in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE, ADC_SINGLECHANNEL);

    // Spin until result ready
    while (ADC_isBusy(ADC_BASE) == ADC_BUSY) ;

    return ADC_getResults(ADC_BASE);
}


/*
 * DVcc is voltage on the digital Vcc pin (versus analog Vcc pin?)
 * In other words, the voltage on the Vcc network in the system.
 *
 * To measure Vcc, ADC read 1.5V bandgap with reference DVcc:
 * To calculate DVCC with 10-bit resolution, use equation: result/1023 = 1.5/DVcc.
 * That equation rearranges to DVCC = (1023 * 1.5) / adcResult.
 *
 * That equation modified to use only integers instead of float. (A time optimization.)
 * The result is in hundreths of volts; needs to be divided by 100 to obtain units volts.
 * DVCC = (1023 * 150) / adcResult
 *
 * For 10-bit resolution need a long (32 bits, max 4G) to hold 1023*150)
 * For 8-bit resolution, need int (16 bits, max 65k) to hold 255*150=38,250
 */
unsigned long ADC::convert10BitADCReadToCentiVolts(unsigned int adcResult) {
    unsigned long result = ((unsigned long) MaxADCRead * (unsigned long) 150)
            / (unsigned long) (adcResult);
    return result;
}

unsigned int ADC::convert8BitADCReadToCentiVolts(unsigned int adcResult) {
    unsigned int result = ((unsigned int) MaxADCRead * (unsigned int) 150)
            / adcResult;
    return result;
}



/*
 * Methods delegated or private.
 */
void ADC::configureForExternalPinVoltageProportionToVcc() {  ADCConfigure::configureForExternalPinVoltageProportionToVcc(); }

unsigned int ADC::readExternalPinVoltageProportionToVcc(){ return read(); }

void ADC::releaseExternalPin(){ ADCConfigure::releaseExternalPin(); }


