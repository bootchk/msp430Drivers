
/*
 * Part of implementation of class RTC.
 * The part dealing with alarm setting.
 * Which is sufficiently complex and unrelated that it is in a separate file.
 */

#include "realTimeClock.h"

#include "RTCInterface.h"
#include "../time/timeConverter.h"

#include "../../softFault/softFault.h"



#include "../../config/driverParameters.h"   // MaxPracticalAlarmDuration

#include "../../assert/myAssert.h"

/*
 * Circular dependency: some RTC methods depend on EpochClock methods which depends on other RTC methods.
 */
#include "../epochClock/epochClock.h"




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
 * Correctness:
 * - minimum duration: duration cannot be so small that we cannot get to sleep before duration elapses
 * - minimum duration: duration cannot be so small that clock ticks beyond alarm while we are setting alarm (Lamport's Rule)
 * - absolute maximum duration: duration cannot be so large that now + duration > clock max
 * - practical max duration: duration cannot be greater than a practical limit defined by the application
 *
 *
 * Minimum duration is two seconds.
 * The code real time guarantees we get to sleep in less than one second.
 * But Lamport's Rule could be violated unless minimum durations is an additional seconds (total of two seconds.)
 * Implementing Lamport's Rule would read the clock after setting alarm to insure clock is not beyond what we read earlier.
 * In lieu of implementing Lamport's Rule, we bump the duration to a minimum of two seconds.
 *
 * Maximum duration:
 * the epoch clock is 32-bits and would roll over after centuries.
 * The duration is also 32-bits and could cause alarm to be past the clock max,
 * but we don't check it, since practical max duration is stronger, and covers this case.
 *
 * Practical max duration:
 * We assume the application specifies a practical max duration.
 * Most application sleeping too long (say longer than a human lifetime) would be pragmatically useless.
 * Purpose for checking practical max duration:
 * - catch software errors in calculating duration.
 * - enforce absolute maximum duration (see above)
 *
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type EpochTime (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 *
 * Two error return cases:
 * - practical max duration exceeded
 * - bus error to external RTC, alarm read does not match alarm written (FUTURE)
 *
 * Also resets if RTC now time is zero (external RTC fails to write data bus)
 * Also asserts if API algebra error (RTC not configured.)
 */
bool RTC::setAlarmDuration(Duration duration) {
	bool result;

	EpochTime alarmEpochTime;

	// Quietly enforce a minimum by bumping upwards
	if (duration.seconds < 2) duration.seconds = 2;

	if (duration.seconds > DriverConstant::MaxPracticalAlarmDuration) return false;

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
    if (RTCInterface::setAlarm(&alarmRTCTime)) {
        // Expensive but should be done
        return verifyAlarmTime(&alarmRTCTime);
    }
    else return false;
}




bool RTC::verifyAlarmTime(const RTCTime* writtenTime) {
    RTCTime readAlarmRTCTime;

    if (RTCInterface::readAlarm(&readAlarmRTCTime)) {
        // equality operator is defined for type RTCTime, passing const RTCTime* on rhs
        return(readAlarmRTCTime == writtenTime);
    }
    else return false;
}

