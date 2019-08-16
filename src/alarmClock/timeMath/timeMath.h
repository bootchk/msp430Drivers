
#pragma once

#include "../time/timeTypes.h"   // EpochTime
#include "../time/interval.h"

/*
 * Math on EpochTime, Duration, Interval
 */

class TimeMath {


public:

    /*
     * Project given <time> in the past by <period> (typically 24 hours, but for testing, less)
     *  until it is beyond <referenceTime> ( which is usually now.)
     * I.E. calculate EpochTime of same wall clock time (0-24) as given time, for the current day.
     */
     static EpochTime projectTimePastReferenceTime(EpochTime time,
                                                   EpochTime referenceTime,
                                                   unsigned long int period);

     /*
      * Project given <time> in the past by <period> (typically 24 hours, but for testing, less)
      * until it is within <halfPeriod> of <referenceTime> ( which is usually now.)
      * E.G. when <period> is 24 hours, calculate EpochTime that is with 12 hours of the <referenceTime>
      * and that is a projection by days of the given <time>.
      *
      * Also returns Interval of projected time from <referenceTime>
      */
     static EpochTime projectTimeByPeriodToNearReferenceTime(const EpochTime time,
                                                             const EpochTime referenceTime,
                                                             const unsigned long int period,
                                                             const unsigned long int halfPeriod,
                                                             Interval& interval);
};
