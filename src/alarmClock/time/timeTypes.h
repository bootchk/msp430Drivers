
#pragma once

/*
 * Notes on time types and math.
 *
 *
 * Duration and EpochTime Math
 *
 * We define Duration as a struct so that EpochTime and Duration cannot be nievely added,
 * even though both are based on a primitive unsigned long (32 bits.)
 * Fixes primitive obsession i.e. eliminate convertability,  EpochTime from Duration types.
 *
 * Addition: EpochTime + Duration.seconds
 *
 *
 *
 * RTCTime, CalendarTime, EpochTime, AlarmTime
 *
 * See AB08xx/rtcTime.h for RTCTime
 *
 * We don't implement AlarmTime struct, it is entirely in RTC hardware, there are hacks when writing it from RTCTime
 *
 * CalendarTime is roughly equivalent to Unix type "tm"
 * EpochTime is roughly equivalent to Unix type "time_t", seconds from an epoch 1970-01-01 00:00:00 +0000 UTC
 *
 *
 *           |  RTCTime |   CalendarTime | AlarmTime
 * ---------------------------------------------------
 * encoding      BCD    |   binary       |   BCD
 *
 * Hundredths    yes        no               yes
 *
 * Weekday        no        no               yes
 *
 * Month [1,12]   yes       yes              yes    * Unix tm type is [0,11]
 */


struct  CalendarTime {
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;

  unsigned char Day;
  unsigned char Month;
  unsigned char Year;   // offset from 1970;
} ;


// unsigned char Wday;   // day of week, sunday is day 1


/*
 * Seconds from an epoch.
 *
 * On most embedded machines, 32 bits, suffers from 2038 problem.
 */
// FUTURE struct with subtraction operator yielding Duration
typedef unsigned long EpochTime;


/*
 * Duration in seconds.
 *
 * 32 bits
 *
 * Typical duration is less than 24 hours.
 * One day is 86,400 seconds.
 * Max unsigned 16 bits is 65,536.
 * Not enforced by this code.
 *
 * A very large duration added to an epoch time could overflow without throwing exception.
 *
 * See notes above about why it is a struct.
 *
 */
//typedef unsigned long Duration;

struct Duration {
    unsigned long seconds;

    /*
     * ???
     */
    //constexpr Duration() = default;

    /*
     * default + parameterized constructor
     */
    Duration(unsigned long seconds = 0) :
            seconds(seconds)
    {
    }

    /*
     * subtraction operator. Doesn't modify object, therefore const.
     *
     * Operand "a" must be less than operand "this".
     * Otherwise, overflow on unsigned arithmetic (yielding a very large number)
     */
    Duration operator-(const Duration& a) const
    {
        if (a.seconds > seconds)
            return Duration(0);
        else
           return Duration(seconds - a.seconds);
    }

    bool operator>(const Duration& a) const
        {
            return (seconds > a.seconds);
        }
} ;









/*
 * Snippets trying to fix primitive obsession i.e. eliminate convertability  EpochTime from Duration types.
 *
 * template<class T> class tagged_integer {  ... }; using row=tagged_integer<struct row_tag>;
 *
 * struct foo {
    using foobar = unsigned long;
};
 *
 */

