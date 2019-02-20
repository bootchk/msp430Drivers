
/*
 * API to control the VBG module
 */

class VBG {
public:
    static void enable();
    /*
     * Ensures low power
     */
    static void disable();
    /*
     * Busy waits until reference voltage is stable.
     */
    static void waitForReady();
};
