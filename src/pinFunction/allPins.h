/*
 * Configures all GPIO as a set.
 */

class AllPins {
public:
    static void setHighOutput();

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
