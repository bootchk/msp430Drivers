
/*
 * Part of ADCConfigure class
 *
 * Configure for reading an external pin.
 */

#include "ADCConfigure.h"
#include "../assert/myAssert.h"

// DriverLib
#include <gpio.h>
#include <adc.h>


// App
#include <board.h>



#ifdef OBSOLETE
/*
didn't work.
Wanted to put pin in low power state.
But pin was powered and it reverse powered mcu through analog pin protection diodes.

So any design must unpower the voltage source being measured.
So this only might be necessary.
*/

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






void ADCConfigure::configureExternalPinVoltagePin() {
    GPIO_setAsPeripheralModuleFunctionInputPin(
            ExternalPinVoltagePort,
            ExternalPinVoltagePin,
            ExternalPinVoltagePinFunction);
    // See board.h Typically,  GPIO_PORT_ADC7,  GPIO_PIN_ADC7, GPIO_FUNCTION_ADC7);
}





void ADCConfigure::configureForExternalPinVoltageProportionToVcc() {

    configureExternalPinVoltagePin();

    configureCommon();

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input defined by <ExternalPinVoltagePinADCSelection>
     * Use positive reference of AVcc
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
            ExternalPinVoltagePinADCSelection,              // <<<<<<<< typically ADC_INPUT_A4
            ADC_VREFPOS_AVCC,
            ADC_VREFNEG_AVSS);

    // conversion might not be enabled.  Reading will enable conversion.
}


void ADCConfigure::configureForExternalPinVoltageProportionTo1_5VBG() {
    configureVoltageBandgapReference();
    configureCommon();

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input pin
     * Use positive reference of AVBG
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
            ExternalPinVoltagePinADCSelection,              // <<<<<<<< typically ADC_INPUT_A4
            ADC_VREFPOS_INT,    // Internal reference VBG
            ADC_VREFNEG_AVSS);

    waitForVoltageBandgapReference();
}

