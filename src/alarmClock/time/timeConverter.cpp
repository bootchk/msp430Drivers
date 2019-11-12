
#include "makeBreakTime.h"
#include "timeConverter.h"


namespace {

/*
 * Each nibble is a decimal digit.
 */
unsigned char bcd2bin(unsigned char value)
{
	return (value & 0x0F) + ((value >> 4) * 10);
}

unsigned char bin2bcd(unsigned char value)
{
	return ((value / 10) << 4) + value % 10;
}

/*
// NOT USED
int yearOfCenturyToTm(int yearOfCentury) {
return 0;
}
*/

} // namespace



/*
 * Conversion offsets the year from different references.
 * Coming from RTC chip, year is offset from century.
 * Epoch time is offset from 1970.
 * We don't really care (as long as we omit conversion in both directions )
 * since no human is reading the results.
 *
 * Omit hundredths, not in Unix type tm
 * calendarTime.Hundreths = bcd2bin(buf.hundreths);
 *
 * !!! Omit superfluous fields:
 * tm_wday
 * tm_yday
 * tm_isdst
 *
 * We are not using the calendar for printing, the typical use for these fields.
 * These fields are not used in reverse conversion (see makeTime() source code)
 */

void TimeConverter::convertRTCTimeToCalendarTime(const RTCTime& rtcTime, CalendarTime& calendarTime) {
    // Not used: CalendarYrToTm(); // /*+ (buf.weekdays.GP * 100)*/) + 2000;

    /*
     * The byte read from register includes high-bit GP bits (general purpose).
     * IOW, the time is in bit-fields, not in bytes.
     * We are not using the GP bits; they should always be zero.
     * But for safety, mask them off anyway.
     */
	calendarTime.Year =  bcd2bin(rtcTime.YearOfCentury );
	calendarTime.Month = bcd2bin(rtcTime.Month        & 0x1F );
	calendarTime.Day =   bcd2bin(rtcTime.DayOfMonth   & 0x3F );
	calendarTime.Hour =   bcd2bin(rtcTime.Hour24      & 0x3F );
	calendarTime.Minute = bcd2bin(rtcTime.Minute      & 0x7F );
	calendarTime.Second = bcd2bin(rtcTime.Second      & 0x7F );
	/*
	 * CalendarTime has no hundredths.
	 */
}


void TimeConverter::convertCalendarTimeToRTCTime(const CalendarTime& calendarTime, RTCTime& rtcTime ) {
    // If rtcTime is written to device, it will clear GP bits.
	rtcTime.YearOfCentury = bin2bcd(calendarTime.Year);
	rtcTime.Month         = bin2bcd(calendarTime.Month);
	rtcTime.DayOfMonth    = bin2bcd(calendarTime.Day);
	rtcTime.Hour24        = bin2bcd(calendarTime.Hour);
	rtcTime.Minute        = bin2bcd(calendarTime.Minute);
	rtcTime.Second        = bin2bcd(calendarTime.Second);
	/*
	 * !!!! Hundredth is defaulted.
	 * But both hundredth and YearOfCentury are not used to set alarm (for some RTC chips.)
	 */
	rtcTime.Hundredth      = 0;
}





EpochTime TimeConverter::convertCalendarTimeToEpochTime(const CalendarTime& calendarTime ) {
	// Roughly equivalent to Unix mktime()
	return makeTime( calendarTime) ;
}


void TimeConverter::convertEpochTimeToCalendarTime( const EpochTime& epochTime, CalendarTime& calendarTime) {
	breakTime( epochTime, calendarTime ) ;
}



/*
 * public
 */

void TimeConverter::convertEpochTimeToRTCTime(const EpochTime& aEpochTime, RTCTime& aRTCTime) {
    CalendarTime aCalendarTime;

    TimeConverter::convertEpochTimeToCalendarTime(aEpochTime, aCalendarTime);

    TimeConverter::convertCalendarTimeToRTCTime(aCalendarTime, aRTCTime);
}


EpochTime TimeConverter::convertRTCTimeToEpochTime(const RTCTime& aRTCTime) {
    CalendarTime aCalendarTime;

    TimeConverter::convertRTCTimeToCalendarTime(aRTCTime, aCalendarTime);
    return TimeConverter::convertCalendarTimeToEpochTime(aCalendarTime);
}



bool TimeConverter::isValidRTCTime(RTCTime & time) {
	/*
	 * Minimal check.
	 * This catches only failure to read RTC chip.
	 * It doesn't check other constraints.
	 *
	 * A valid day of month as read from RTC is [1,31]
	 * RTCTime is encoded BCD, so actual value is[0x01,.., 0x09,.., 0x19, ..., 0x31]
	 */
	return (time.DayOfMonth > 0 );
}
