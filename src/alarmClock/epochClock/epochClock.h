
#include "../time/timeTypes.h"   // EpochTime, Duration


/*
 * EpochTime clock implemented on RTC.
 *
 * Requires RTC accessible.  When RTC is external chip, requires bus to RTC configured prior.
 *
 * The RTC clock runs.
 * EpochClock does not run, it only reads the RTC to compute a now time as necessary.
 *
 * The RTC starts at 0.
 * This clock's epoch is the real time the RTC is started.
 *
 * The first call to timeNow() will return a small value,
 * since the RTC must be started a short duration before the first call to timeNow().
 *
 * !!! Depends on RTC.  Since it is external, over a bus (e.g. I2C) it may fail hard.
 * Most methods may reset the MCU if external RTC chip fails hard.
 */


class EpochClock {
    /*
     * FUTURE:
     * Return current time of this waking period.
     * Memoized: repeated calls during this waking period returns the same time.
     * If the waking period is longer than a second, return time might not be the real time
     * (might not match time retrieved from RTC.)
     */

public:
    /*
     * Get time from RTC, as of the call.
     * Resets if external RTC fails hard.
     */
    static EpochTime timeNowOrReset();

    /*
     * Return EpochTime that is Duration seconds in the future from now.
     * Resets if external RTC fails hard.
     */
    static EpochTime timeDurationFromNowOrReset(Duration);

    /*
     * Return EpochTime that is Duration seconds later than given EpochTime.
     * Neither the given time nor the returned time are guaranteed to be in the future from now.
     * This is just math.
     * Does not depend on RTC and will not reset.
     */
    static EpochTime timeDurationFromTime(EpochTime, Duration);

    /*
     * Ensure that alarm can be set for the given time?
     * An alarm should not be set for a time in the past,
     * or a time that is too near in the future.
     *
     * If given time is not alarmable,
     * sets the given time to the soonest time in the future for which an alarm can be set.
     */
    static void setTimeAlarmableInFuture(EpochTime&);


    // FUTURE
    /*
     * Is the given time in the future as of the time of the call?
     * If used to check a time when setting alarm, should be called ASAP before setting alarm.
     */
    static bool isTimeAlarmable();
};
