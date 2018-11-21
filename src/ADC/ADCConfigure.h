
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

public:
    // Both configurations of external pin hijack the GPIO function of the pin

    // configure to measure external pin proportion to Vcc
    // Since Vcc varies, this doesn't give an absolute voltage
    static void configureForExternalPinVoltageProportionToVcc();

    // configure to measure external pin proportion to 1.5V band gap ref
    // The VBG also varies slightly with Vcc, but this is more accurate than the above.
    static void configureForExternalPinVoltageProportionTo1_5VBG();

    // configure to measure Vcc in centiVolts
    // Does not hijack GPIO
    static void configureForVccMeasure();

    // Undo ADC hijacking the GPIO function of pin
    static void releaseExternalPin();
};
