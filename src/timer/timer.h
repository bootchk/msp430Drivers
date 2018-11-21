
/*
 * Delay with cpu idle but RAM retained.
 *
 * cpu idle:
 * AKA standby
 * AKA LPM3 (TI nomenclature)
 *
 * !!! Requires separate ISR for RTC.
 * The ISR must exit:
 * - clear interrupt flag for overflow
 * - exit LPM on exit
 */

class LowPowerTimer {
public:

    static void delayTicksOf100uSec(unsigned int);

    static void delayHalfSecond();
    static void delayTwentyMilliSeconds();
    static void delayFiveSeconds();
};
