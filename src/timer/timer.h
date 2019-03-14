
/*
 * Delay with cpu idle but RAM retained (LPM3).
 *
 * cpu idle:
 * AKA standby
 * AKA LPM3 (TI nomenclature)
 *
 * !!! Requires separate ISR.
 * The ISR is not in this project because it then would not get linked in.
 * The ISR must:
 * - clear interrupt flag for overflow
 * - exit LPM on exit
 *
 * The implementation:
 * - uses a clock (usually VLO)and an internal module (say the RTC.)
 * - starts them and shuts them down for each call.
 *
 * There are two different implementations:
 * - RTC (a specific module and version, an older one)
 * - WDT (WDT_A in TI nomenclature)
 * These are not available on some family members.
 * E.G. some family members have RTC_C, distinct from RTC.
 *
 * Both the WDT and the RTC are available in LPM3.
 * Even though some block diagrams only show the RTC in the "LPM3.5 domain."
 * It is not clear whether WDT is running in LPM3.5 (but its moot here, this is for LPM3)
 *
 * In other words, this API is intended to be general,
 * for two different implementations.
 */

class LowPowerTimer {
public:

    /*
     * Ensures postcondition clock and module are off, or allowed to go off.
     * E.G. VLO and RTC   or VLO and WDT interval timer
     */
    static void delayTicksOf100uSec(unsigned int);

    // Convenience functions that call the above.
    static void delayFiveSeconds();
    static void delayHalfSecond();
    static void delayTwentyMilliSeconds();
    static void delayTenMilliSeconds();

    static void delaySeconds(unsigned int count);
};
