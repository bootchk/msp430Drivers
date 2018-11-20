#pragma once

/*
 * Facade to Power Management Module
 *
 * Here mostly about low power mode.
 * See User's Guide: 1.4.3 "Low-Power Modes...LPMx.5"
 */
class PMM {
public:
    /*
     * !!! BOR is a named event in mcu state diagram, equivalent to power-on reset
     * !!! POR is a distinct event, not equivalent to power-on reset.
     */
    static void triggerSoftwareBORReset();

	/*
	 * Configure PMM for least power in sleep.
	 */
	static void configureOff();



	/*
	 * Take mcu from awakeLPM5 to active state, with GPIO pins unlocked, and any prior
	 *
	 * State of mcu w/r to LPM5:
	 * -sleepingLPM5: cpu off but GPIO pins locked.
	 * -awakeLPM5: cpu on but GPIO pins locked.  Any GPIO configuration done in this state is not effective.
	 * -active: cpu on and GPIO pins unlocked
	 */
	static void unlockLPM5();
	static bool isLockedLPM5();

	/*
	 * A flag set by HW, checkable, and must be cleared.
	 */
	static bool isResetAWakeFromSleep();
	static void clearIsResetAWakeFromSleep();
	/*
	 * Clear all flags for any reset reasons.
	 */
	static void clearAllResetInterruptFlags();
};
