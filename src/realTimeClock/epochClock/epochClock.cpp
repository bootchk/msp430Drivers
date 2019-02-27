

#include "epochClock.h"

#include "../realTimeClock.h"



EpochTime EpochClock::timeNow() {
    return RTC::timeNowOrReset();
}



EpochTime EpochClock::timeDurationFromNow(Duration duration) {
    return timeDurationFromTime(timeNow(), duration);
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

    EpochTime alarmableTime = timeNow() + 2;
    if (time < alarmableTime )
        time = alarmableTime;
}




