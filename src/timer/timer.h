
/*
 * Delay with cpu idle but RAM retained.
 *
 * cpu idle:
 * AKA standby
 * AKA LPM3 (TI nomenclature)
 *
 * !!! Requires separate ISR for RTC.
 * The ISR is not in this project because it then would not get linked in.
 * The ISR must:
 * - clear interrupt flag for overflow
 * - exit LPM on exit
 *
 * The implementation:
 * - uses the VLO clock and the internal RTC module.
 * - starts them and shuts them down for each call.
 */

class LowPowerTimer {
public:

    /*
     * Ensures postcondition VLO and RTC are off.
     */
    static void delayTicksOf100uSec(unsigned int);

    // Convenience functions that call the above.
    static void delayFiveSeconds();
    static void delayHalfSecond();
    static void delayTwentyMilliSeconds();
    static void delayTenMilliSeconds();

};
