
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
     * Decodes and clears all reasons for sleep.
     */
    static bool isResetAWakeFromSleep();

    /*
     * Clear IFG for wake from LPMx.5
     */
    static void clearIsResetAWakeFromSleep();

    /*
     * Unlocks GPIO state and makes effective any prior configuration.
     * GPIO can be configured prior, but said configuration is not effective until this call.
     *
     * Must follow isResetAWakeFromSleep
     * Once unlocked, you can't tell whether reset was from sleep.
     */
    static void unlockMCUFromSleep();


    static void triggerSoftwareReset();
};
