
/*
 * API to control the VBG module
 *
 * Note the MSP430FR2433 has a "basic" voltage reference, not "enhanced".
 *
 * Thus 2.0, 2.5V references not available.
 * Also, no BGMODE appears to be available.
 */

class VBG {
public:
    /*
     * Switch VRef to the ADC.
     *
     * Not sure if this triggers any other signals, say off and on.
     */
    static void enable();
    /*
     * Ensures low power???
     */
    static void disable();
    /*
     * Busy waits until reference voltage generator is stable.
     *
     * Implementation is via reference generator.
     * Not sure whether we also need to check readiness of VBG.
     */
    static void waitForReady();
};
