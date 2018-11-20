
/*
 * Knows how to convert time formats.
 *
 * !!! With many subtle omissions.
 *
 * Goal here is to use Unix std implementations when possible.
 */

/*
 * A library similar to Unix std library
 *
 * If the platform support Unix std library:
 * #include <ctime>	// C time.h
 */
#include "rtcTime.h"
#include "timeTypes.h"



class TimeConverter {
private:
	/*
	 * Convert BCD encoded calendar into int encoded calendar.
	 * and vice versa
	 */
	static void convertRTCTimeToCalendarTime(const RTCTime&, CalendarTime& );
	static void convertCalendarTimeToRTCTime(const CalendarTime&, RTCTime& );

	/*
	 * Convert calendar time to epoch time and vice versa
	 */
	static EpochTime convertCalendarTimeToEpochTime(const CalendarTime&);
	static void convertEpochTimeToCalendarTime(const EpochTime&, CalendarTime&);

public:

	static void convertEpochTimeToRTCTime(const EpochTime&, RTCTime&);
	static EpochTime convertRTCTimeToEpochTime(const RTCTime&);


	static bool isValidRTCTime(RTCTime&);
};
