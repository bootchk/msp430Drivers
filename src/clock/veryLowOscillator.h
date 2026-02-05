
/*
 * Thin wrapper around TI MSP430 VLO oscillator
 */

class VeryLowOscillator {
public:

    static void ensureOn();

    /*
    Let VLO stop itself after last module requests it.
    
    Reset default is allowOff, so this is unneccessary
    when ensureOn was never called.
     */
    static void allowOff();
};
