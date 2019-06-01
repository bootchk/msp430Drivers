#pragma once


/*
 * Routines for testing
 *
 * Specific to a dev board.
 */

/*
 * Each log entry is one or two words.
 * One is always one of these codes.
 * The other word (may precede the code) is more information.
 */
enum class FailCode {
    // C++ enums start at value 0
    HWFault,    // HW bus error e.g. VMA

    Assert,      // assertion false,  Also logs line number.

    // SW detected unrecoverable error
    SWFaultSetAlarm,
    SWFaultClearAlarm,
    SWFaultReadTime,
    SWFaultDetectLight,
    SWUnhandledReset,   // Also logs resetReason

    // SW detected, info about dire situation.  Also logs an info code
    SWInfo
};


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


    // Attempt to break into the debugger, then infinite loop.

    static void stop(FailCode);
    /*
     * Fatal conditions
     */
    // Discovered in ordinary code
    // Call comes from programmer written error checking that is never removed from build.
    static void fatalSWFault(FailCode);

    // Discovered by CPU e.g. bus error or vacant memory address
    // Call comes from the NMI ISR
    static void fatalHWFault();

    // Discovered by myAssert()
    // asserts can be disabled (removed from the build.)
    static void fatalAssert(unsigned int);
};
