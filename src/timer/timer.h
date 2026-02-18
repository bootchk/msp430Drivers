
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
 * - clear any interrupt flag (overflow for RTC, none for WDT)
 * - exit LPM on exit
 *
 * The implementation:
 * - uses a clock (usually VLO)and an internal module (say the RTC or WDT.)
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
 *
 * All durations are VERY approximate.
 * For some implementations, accuracy is less than 50%.
 * E.G. one second may be 0.6 seconds.
 */

class LowPowerTimer {
public:

    /*
     * Does not return until duration is elapsed.
     *
     * Is LPM3 mostly until returns.
     *
     * Ensures postcondition clock and module are off, or allowed to go off.
     * E.G. VLO and RTC   or VLO and WDT interval timer
     */
    static void delayTicksOf100uSec(unsigned int);


    // Some are convenience functions that iterate and call others.
    // Depends on implementation.
    // Do not assume the delay is one continuous period in LPM.

    // When LowPowerTimer uses WDT, these are continuous periods
    static void delay48Seconds();
    static void delayThreeSeconds();
    static void delaySecond();
    static void delayTenMilliSeconds();

    // Can be iterated periods in LPM
    // Convenience
    static void delayFiveSeconds();
    static void delayHalfSecond();
    static void delayTwentyMilliSeconds();
    static void delayTwoMilliSeconds();

    static void delaySeconds(unsigned int count);
};
