
/*
 * Understands how to configure the ADC
 *
 * Note that using the ADC may hijack GPIO pins (register SYSCFG2)
 * The module/function selection for a GPIO is not used to select the analog pin use of the pin.
 * These routines can leave the GPIO module selection intact.
 * If you are not multiplexing a pin for both GPIO and analog, you can ignore this complexity.
 */
#pragma once



class ADCConfigure {
    // Configuration common to the above
    static void configureCommon();

    /*
     * used to measure Vcc.
     * Does NOT ensure it is ready, it needs delay to warm up.
     */
    static void configureVoltageBandgapReference();

    static void waitForVoltageBandgapReference();

    static void disableVoltageBandgapReference();


    // Configure the GPIO function of the pin to be read by ADC
    static void configureExternalPinVoltagePin();



public:
    // External pin as defined by board.h.
    // Both methods hijack the GPIO function of the pin

    // configure to measure external pin proportion to Vcc
    // Since Vcc varies, this doesn't give an absolute voltage
    static void configureForExternalPinVoltageProportionToVcc();

    // configure to measure external pin proportion to 1.5V band gap ref
    // The VBG also varies slightly with Vcc, but this is more accurate than the above.
    static void configureForExternalPinVoltageProportionTo1_5VBG();

    // Undo ADC hijacking the GPIO function of pin
    static void releaseExternalPin();





    // configure to measure Vcc in centiVolts
    // Does not hijack GPIO
    static void configureForVccMeasure();
    /*
     * Not sure this is necessary?
     * Make the ADC low power.
     * The ADC will automatically be low power after a single read.
     * But the voltage bandgap reference in the PMM might draw power, so this disables it.
     */
    static void unconfigureForVccMeasure();





    /*
     * Unpower the ADC module.
     * Requires isConversionDisabled()
     */
    static void disableADC();
    /*
     * !!! Set ADC module to a mode that allows configuration and turning off.
     * We don't have enableConversions() because the DriverLib implementation of startConversion
     * automatically enables conversion.
     */

    static void disableConversions();
    static bool isConversionDisabled();
};
