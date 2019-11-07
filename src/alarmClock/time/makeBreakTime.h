
/*
 * Conversions between time types
 */

#include "timeTypes.h"


// Roughly equivalent to Unix localtime()
// Break EpochTime into struct CalendarTime
extern void breakTime(const EpochTime& timeIn, CalendarTime &timeOut);

// Roughly equivalent to Unix mktime()
// Make an EpochTime from a CalendarTime
extern EpochTime makeTime(const CalendarTime &);
