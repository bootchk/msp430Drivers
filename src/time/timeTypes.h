
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

typedef struct  {
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;

  unsigned char Day;
  unsigned char Month;
  unsigned char Year;   // offset from 1970;
} CalendarTime;


// unsigned char Wday;   // day of week, sunday is day 1


/*
 * Seconds from an epoch.
 *
 * On most embedded machines, 32 bits, suffers from 2038 problem.
 */
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

typedef struct {
    unsigned long seconds;
} Duration;

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

