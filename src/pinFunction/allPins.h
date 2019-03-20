/*
 * Configures all GPIO as a set.
 */

class AllPins {
public:
    static void setOutput();
    static void setValue(unsigned char);

    /*
     * Configure all GPIO to output with given value.
     */
    static void setHighOutput();
    static void setLowOutput();

    /*
     * Are all GPIO pins configured for their general purpose IO function (not a module.)
     */
    static bool areGeneralPurpose();


    /*
     * May assert and not return.
     * One interrupt pin.
     */
    static void assertAreConfiguredForSleep();
};
