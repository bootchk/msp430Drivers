
/*
 * Voltage measurement using ADC.
 *
 * This is high-level.  Not "reads" but "measures".
 * Not designed for multiple, consecutive measurements of the same input.
 * Each "measure" initializes, configures, and completely turns off the ADC.
 *
 * Configuration is a lower level, see ADCConfigure,
 * which configure resources ( VBG, ADC and pin multiplexing) to support the above.
 *
 * Responsibilities:
 * - measure Vcc in centiVolts
 * - measure proportion of V on external pin to various references
 * -- 1.5VBG
 * -- Vcc
 * - conversion of raw results to units centiVolts
 *
 * A proper use of these routines can leave all GPIO's in their original use.
 *
 * 8-bit resolution only.
 *
 * Algebra of calls:  measureVccCentiVolts, ....
 *
 * That is the only thing working.
 * Again, it leaves the ADC in sleeping state (off) after every call!!!
 */


/*
 * OLD needs revamping, most of this moved to ADCConfigure
 *
 *
 * TODO disable VBG to save power?
 * The ADC will not draw power when not being sampled.
 *
 * To use this requires:
 * - certain sequences of method calls
 * - certain timings
 * See algebra below.
 *
 * Algebra:
 *
 * In general the sequence is configure(), delay() or wait(), read(), unconfigure()
 *
 * When measuring Vcc: measureVccCentiVolts()
 * no GPIO is altered, a slight internal delay, and no resources active afterwards.
 *
 * When measuring external pin versus 1.5V: measureExternalPinProportionTo1_5VBG();
 * slight internal delay
 * no delay for pin signal to stabilize
 * GPIO left in original configuration
 *
 *
 * When measuring an external pin dedicated to a signal (and with no sleep that would reset configuration):
 * Sequence: configure() (once), read(), read(), ...
 *
 * When measuring an external pin that is multiplexed with GPIO:
 * Sequence: configure(), delay() (for signal to stabilize) , read(),..., read(), unconfigure(), <use pin for GPIO>
 *
 * Unconfiguration can be moot when you are entering LPMx.5, which resets configuration anyway???
 *
 *
 */


class ADC {

    /*
     * Sample ADC using existing configuration.
     */
    static unsigned int read();

    static unsigned long convert10BitADCReadToCentiVolts(unsigned int);
    static unsigned int convert8BitADCReadToCentiVolts(unsigned int);


public:
    // Does not require configure or release GPIO pin
    static unsigned int measureVccCentiVolts();

    // May hijack external pin, and may not be accurate in that case
    static unsigned int measureExternalPinProportionTo1_5VBG();
    static unsigned int measureExternalPinProportionToVcc();

    /*
     * When external pin also in use for GPIO,
     * require a configure(), <delay>, read(), unconfigure()
     * I.E. expose methods of ADCConfigure and private read()
     *
     * The delay might be necessary in certain cases of GPIO use???
     * When not GPIO out, might not be any capacitance on internal buffers?
     */
    static void configureForExternalPinVoltageProportionToVcc();
    static unsigned int readExternalPinVoltageProportionToVcc();
    static void releaseExternalPin();

};
