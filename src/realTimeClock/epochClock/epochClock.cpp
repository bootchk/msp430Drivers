

#include "epochClock.h"

#include "../realTimeClock.h"


namespace {

/*
 * Memoized time of this waking period.
 *
 * !!! The clock is still ticking.
 * !!! And this is memoized.
 * It is NOT the real time of the RTC clock.
 *
 * This code is not general, but assumes minimums on alarm durations and wake period duration.
 * In other words, Lamport's Rule is not used to set the alarm
 * (Lamport's rule: set alarm, then check that clock has not ticked and alarm is thus in the past.)
 *
 * C startup clears this each wake period
 */
EpochTime timeNow = 0;

}


EpochTime EpochClock::timeNowOrReset() {

    if (timeNow == 0) {
        // Have not called RTC::timeNowOrReset() in this waking period

        // This requirement is valid but implemented lower down.
        // myRequire(RTC::isConfigured);
        timeNow = RTC::timeNowOrReset();
    };
    return timeNow;
}



EpochTime EpochClock::timeDurationFromNowOrReset(Duration duration) {
    return timeDurationFromTime(timeNowOrReset(), duration);
}

EpochTime EpochClock::timeDurationFromTime(EpochTime time, Duration duration) {
    return time + duration.seconds;
}





/*
 * A time that is only one second in the future may occur at any time (in the next microsecond,
 * since the clock may tick the next second at any time.
 * A time must be two seconds in the future to ensure that the time is at least one real second in the future.
 */
 void EpochClock::setTimeAlarmableInFuture(EpochTime& time) {

    EpochTime alarmableTime = timeNowOrReset() + 2;
    if (time < alarmableTime )
        time = alarmableTime;
}
