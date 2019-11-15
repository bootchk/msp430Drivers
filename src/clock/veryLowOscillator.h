
/*
 * Thin wrapper around TI MSP430 VLO oscillator
 */

class VeryLowOscillator {
public:

    // NOT USED static void start();

    /*
     * Allow stop after last module requests it.
     *
     * Reset default is allowOff, so this is unneccessary.
     */
    static void allowOff();
};
