
/*
 * Part of implementation of class RTC.
 * The part dealing with alarm setting.
 * Which is sufficiently complex and unrelated that it is in a separate file.
 */

#include "realTimeClock.h"

//#include "../bridge/bridge.h"
#include "RTCInterface.h"
#include "../softFault/softFault.h"

#include "../time/timeConverter.h"

/*
 * Circular dependency: some RTC methods depend on EpochClock methods which depends on other RTC methods.
 */
#include "epochClock/epochClock.h"


#ifdef NOT_USED
namespace {

/*
 * Time is not zero and greater than previous given time.
 */

bool timeIsMonotonic(EpochTime nowTime) {
	// FUTURE compare to previous read time, must be greater.
	return true;
}

}
#endif




/*
 * !!! If the RTC stops responding, system is failed, there is no practical action to take.
 * During testing, without proper connection to RTC,
 * this code seems to function since reads and writes to RTC will work as far as mcu can tell,
 * except that reads will return zero e.g. now time will be zero.
 */



EpochTime RTC::timeNowOrReset() {
    RTCTime now;

    RTCInterface::readTime(&now);

    /*
     * If RTC has failed, Bridge reads time as all zeroes.
     * RTC failed: master is clocking but slave RTC is failing to drive data line.
     * And possibly failing to drive ACK bit.
     */
    if (not TimeConverter::isValidRTCTime(now)) {
        SoftFault::failReadTime();
    }

    return TimeConverter::convertRTCTimeToEpochTime(now);
}




/*
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type EpochTime (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
bool RTC::setAlarmDuration(Duration duration) {
	bool result;

	/*
	 * FUTURE check preconditions for setting alarm:
	 * duration is great enough so that we get to sleep
	 * before the alarm goes off.
	 *
	 * For now, the minimum duration is one second,
	 * and assume that sleep follows setAlarm by much less than one second.
	 */

	EpochTime alarmEpochTime;

	alarmEpochTime = EpochClock::timeDurationFromNowOrReset(duration);

    // Fails if alarm not verifiably written to remote RTC device
    result = setAlarmTime(alarmEpochTime);

	return result;
}


bool RTC::setAlarmTime(EpochTime alarmEpochTime) {
    RTCTime alarmRTCTime;

    // takes reference to alarmRTCTime
    TimeConverter::convertEpochTimeToRTCTime(alarmEpochTime, alarmRTCTime);

    // Takes a pointer, not a reference
    RTCInterface::writeAlarm(&alarmRTCTime);

    return true;
    // FUTURE verify alarm written
    ///return verifyAlarmTime(&alarmRTCTime);
}




bool RTC::verifyAlarmTime(const RTCTime* writtenTime) {
    RTCTime readAlarmRTCTime;

    RTCInterface::readAlarm(&readAlarmRTCTime);
    return(readAlarmRTCTime == writtenTime);
}

