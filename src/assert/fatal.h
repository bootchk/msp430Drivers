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

    // Do a software reset and log given reason
    static void reboot(unsigned int reason);

    /*
     * Fatal conditions
     */
    // Discovered in ordinary code
    static void fatalSWFault();

    // Discovered by CPU e.g. bus error or vacant memory address
    static void fatalHWFault();

    // Discovered by myAssert()
    static void fatalAssert(unsigned int);
};
