/*
 * Configures all GPIO as a set.
 */

class AllPins {
public:
    // Class methods (static)

    /*
     * Configure all gpio pins to be outputs.
     * (So they will use low power when sleeping.)
     */
    // configure all GPIO out to ensure low power
    static void configureGPIOLowPower();
    // alternative, faster implementation
    static void setOutput();
    static void setValue(unsigned char);

    /*
     * Configure all GPIO to output with given value.
     */
    static void setHighOutput();
    static void setLowOutput();


    /*
     * Alternative implementation, slower.
     * Configure all gpio pins to have low values (when configured as outputs.)
     */
    static void setAllOutputsLow();

    /*
     * Are all GPIO pins configured for their general purpose IO function (not a module.)
     */
    static bool areGeneralPurpose();


    /*
     * A special function, not generic.
     * Special for embeddedDutyCycle framework and a particular application.
     * I.E. knows pins framework and app uses.
     *
     * It is redundancy, for use during testing.
     *
     * May assert and not return.
     */
    static void assertAreConfiguredForSleep();
};
