
#include "../../time/timeTypes.h"   // EpochTime, Duration


/*
 * EpochTime implemented on RTC.
 */


class EpochClock {
public:
    static EpochTime timeNow();
    static EpochTime timeDurationFromNow(Duration);
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
