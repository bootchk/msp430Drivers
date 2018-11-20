#pragma once


/*
 * Routines for testing
 *
 * Specific to a dev board.
 */


class Fatal {

public:
    /*
     * Ensure green LED change to OUT will be visible.
     * !!! Regardless of cpu LOCKLPM5 state and prior GPIO configuration
     */
    static void ensureGreenLEDLightable();
    static void ensureRedLEDLightable();


    /*
     * Warble LED.  Does not return.
     */
    static void warbleGreenLEDForever();
    static void warbleRedLEDForever();

    /*
     * Light green LED.  Does not return.
     */
    static void abortGreenLED();

    /*
     *
     */
    static void reboot();
    static void fatalReset();
    static void fatalAssert(unsigned int);
};
