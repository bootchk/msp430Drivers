/*
 * Thin wrapper around TI MSP430 VLO oscillator
 */

class SubmainClock {
public:

    /*
     * Not ensure ON
     */
    static void init();


    static void turnOn();
};
