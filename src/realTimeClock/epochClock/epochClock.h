
#include "../../time/timeTypes.h"   // EpochTime, Duration


/*
 * EpochTime clock implemented on RTC.
 *
 * The RTC clock runs.
 * This clock does not run, it only reads the RTC to compute a now time as necessary.
 *
 * The RTC starts at 0.
 * This clock's epoch is the real time the RTC is started.
 *
 * The first call to timeNow() will return a small value,
 * since the RTC must be started a short duration before the first call to timeNow().
 */


class EpochClock {
public:
    static EpochTime timeNow();

    /*
     * Return EpochTime that is Duration seconds in the future from now.
     */
    static EpochTime timeDurationFromNow(Duration);
    /*
     * Return EpochTime that is Duration seconds later than given EpochTime.
     * Neither the given time nor the returned time are guaranteed to be in the future from now.
     */
    static EpochTime timeDurationFromTime(EpochTime, Duration);

    /*
     * Ensure that alarm can be set for the given time?
     * An alarm can not be set for a time in the past,
     * or a time that is too near in the future.
     *
     * If given time is not alarmable,
     * sets the given time to the soonest time in the future for which an alarm can be set.
     */
    static void setTimeAlarmableInFuture(EpochTime&);
};
