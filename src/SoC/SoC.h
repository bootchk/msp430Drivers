
/*
 * Facade for controlling SoC components:
 * system core
 * bus faults
 * watchdog
 *
 * These deal with the SoC (system on chip) not just the mcu.
 * The watchdog, PMM, etc are not part of the mcu.
 */

class SoC {
public:
    /*
     * Stop watchdog, which is enabled on reset.
     * Thus usually during boot, this is called early.
     */
    static void stopWatchDog();

	static void enterLPM4orLPM4_5();

	static void enableGlobalInterrupts();

	/*
	 * Make BSL behave like vacant memory
	 * and enable vacant memory non maskable interrupt
	 */
	static void enableBSLOffAndVacantMemoryNMI();


	/*
	 * Disable write protect for all FRAM.
	 */
	static void disableFRAMWriteProtect();

    /*
     * Called after mcu has been reset.
     * True if sleeping before this reset.
     * False if not sleeping, and therefore a power on or software reset or other reset.
     *
     * Decodes and clears all reasons for sleep, in the IVG interrupt vector generator.
     * Does not clear the IFG for wake from sleep.
     *
     * May assert if reset reason is not an expected one,
     * e.g. some fault and not a wake from sleep reset or a coldstart reset.
     */
    static bool isResetWakeFromSleep();

    /*
     * Clear IFG for wake from LPMx.5
     */
    static void clearIFGForResetWakeFromSleep();

    /*
     * Unlocks GPIO state and makes effective any prior configuration.
     * GPIO can be configured prior, but said configuration is not effective until this call.
     *
     * Must follow isResetAWakeFromSleep
     * Once unlocked, you can't tell whether reset was from sleep.
     */
    static void unlockGPIOFromSleep();


    static void triggerSoftwareReset();

    /*
     * Disable XT1AutoOff.
     * XT1AutoOff defaults to enabled.
     * This function is stronger: ensures that XT1 is off unless it is selected in Port function.
     * lkk Not sure this is necessary.  Added this function so I could test whether it affected LPM4.5.
     */
    static void disableXT1();


    static void turnOffSMCLK();
};
