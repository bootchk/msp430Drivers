
#include "ADCConfigure.h"

#include "voltBandgapRef.h"

// DriverLib
#include <gpio.h>
#include <adc.h>

#include "../assert/myAssert.h"





/*
 * Private
 */




void ADCConfigure::configureCommon() {
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use internal ADC bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);
    // assert ADC is off

    ADC_enable(ADC_BASE);
    // assert ADC is on

    /*
     * Base Address for the ADC Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
       ADC_CYCLEHOLD_4_CYCLES, //ADC_CYCLEHOLD_16_CYCLES, 4, 64
       ADC_MULTIPLESAMPLESDISABLE);

    // Default read-back format is unsigned

    ADC_setResolution(ADC_BASE,
            ADC_RESOLUTION_8BIT);

    // Initialization is not complete until caller configures input
}




/*
 * Public
 */

// See adc.h for these constants for input sources
//ADC_INPUT_DVSS

void ADCConfigure::configureForVccMeasure() {
    VBG::enable();
    // We wait for ready later

    configureCommon();

    /*
     * To measure Vcc,
     * sample the 1.5V bandgap reference from the PMM
     * with AVCC as input reference.
     * The sampled bandgap varies in proportion to Vcc.
     */
    ADC_configureMemory(ADC_BASE,
                ADC_INPUT_REFVOLTAGE,              // <<<<<<<<
                ADC_VREFPOS_AVCC,
                ADC_VREFNEG_AVSS);

    VBG::waitForReady();
    // Assert ready to read
}

void ADCConfigure::unconfigureForVccMeasure() {
    /*
     * The ADC itself will enter low power automatically,
     * but we must disable the VBG???
     */
    VBG::disable();

    disableADC();
}







/*
 * Change the state of the ADC.
 * off->on->conversionEnabled->conversionDisabled->off
 */

void ADCConfigure::disableADC() {
    // require conversion disabled
    myRequire(isConversionDisabled());
    ADC_disable(ADC_BASE);
}


bool ADCConfigure::isConversionDisabled() {
    return not (ADCCTL0 & ADCENC);
}

void ADCConfigure::disableConversions() {
    ADC_disableConversions(ADC_BASE, false);    // false=>no prempt
}




