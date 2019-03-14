
/*
 * Thin wrapper around TI MSP430 VLO oscillator
 */

class VeryLowOscillator {
public:

    static void start();

    /*
     * Allow stop after last module requests it.
     */
    static void allowOff();
};
