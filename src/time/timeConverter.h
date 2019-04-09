
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


/*
 * FUTURE: optimization.
 * Since the RTC chip we use doesn't alarm on Years,
 * we could omit the Year conversions when used for setting alarm.
 */
#include "rtcTime.h"
#include "timeTypes.h"



class TimeConverter {
private:
	/*
	 * Convert BCD encoded RTCTime to and from int encoded calendar.
	 *
	 * !!! Note that RTCTime has extra field hundredths, which we don't convert.
	 * So converting from CalendarTime to RTCTime yields an RTCTime with defaulted field hundredths.
	 */
	static void convertRTCTimeToCalendarTime(const RTCTime&, CalendarTime& );
	static void convertCalendarTimeToRTCTime(const CalendarTime&, RTCTime& );

	/*
	 * Convert calendar time to epoch time and vice versa.
	 *
	 * Conversion is exact.
	 */
	static EpochTime convertCalendarTimeToEpochTime(const CalendarTime&);
	static void convertEpochTimeToCalendarTime(const EpochTime&, CalendarTime&);

public:

	static void convertEpochTimeToRTCTime(const EpochTime&, RTCTime&);
	static EpochTime convertRTCTimeToEpochTime(const RTCTime&);


	static bool isValidRTCTime(RTCTime&);
};
