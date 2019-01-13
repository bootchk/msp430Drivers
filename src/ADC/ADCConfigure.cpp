
#include "ADCConfigure.h"

// DriverLib
#include <gpio.h>
#include <adc.h>
#include <pmm.h>

// App
#include <board.h>


/*
 * Private
 */

#ifdef OBSOLETE
// didn't work, reverse powered mcu through analog pin
// TODO, not tied to board.h
void ADCConfigure::releaseExternalPin() {
    // bit clear A0
    SYSCFG2 &= ~BIT0;
    /*
     * assert configuration and release of an external GPIO pin for ADC use
     * does not alter the configuration of the GPIO pin
     * e.g. if it was GPIO out, it still is
     */
}
#endif


void ADCConfigure::waitForVoltageBandgapReference() {
    // spin
    while (not PMM_getVariableReferenceVoltageStatus == PMM_REFGEN_READY) ;
}

void ADCConfigure::configureVoltageBandgapReference() {
    PMM_enableInternalReference();
}

// TODO disable???


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

    ADC_enable(ADC_BASE);

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
}




/*
 * Public
 */

// See adc.h for these constants for input sources
//ADC_INPUT_DVSS

void ADCConfigure::configureForVccMeasure() {
    configureVoltageBandgapReference();
    // We wait for it later

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

    //
    waitForVoltageBandgapReference();
    // Assert ready to read
}


#ifdef OBSOLETE
void ADCConfigure::configureForExternalPinVoltageProportionToVcc() {
    configureCommon();

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A7
     * Use positive reference of AVcc
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
            ExternalPinVoltagePinADCSelection,              // <<<<<<<< typically ADC_INPUT_A4
            ADC_VREFPOS_AVCC,
            ADC_VREFNEG_AVSS);
}


void ADCConfigure::configureForExternalPinVoltageProportionTo1_5VBG() {
    configureVoltageBandgapReference();
    configureCommon();

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A7
     * Use positive reference of AVBG
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
            ExternalPinVoltagePinADCSelection,              // <<<<<<<< typically ADC_INPUT_A4
            ADC_VREFPOS_INT,    // Internal reference VBG
            ADC_VREFNEG_AVSS);

    waitForVoltageBandgapReference();
}
#endif
