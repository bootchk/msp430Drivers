
#pragma once

/*
 * Notes on time types and math.
 *
 * We define Duration as a struct so that EpochTime and Duration cannot be nievely added,
 * even though both are based on a primitive unsigned long (32 bits.)
 * Fixes primitive obsession i.e. eliminate convertability,  EpochTime from Duration types.
 *
 * Addition: EpochTime + Duration.seconds
 */
/*
 * See also AB08xx/rtcTime.h for RTCTime
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
} ;



/*
 * Difference of two EpochTimes.
 * Signed.
 * Absolute value is a Duration.
 *
 * No enforcement of overflow.
 */
typedef signed int Interval;

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

