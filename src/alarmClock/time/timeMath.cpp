
#include "timeMath.h"

#include "../time/interval.h"

#include "../../assert/myAssert.h"


EpochTime TimeMath::projectTimePastReferenceTime(EpochTime timeInPast,
                                                  EpochTime referenceTime,
                                                  unsigned long int period) {
    /*
     * One design is to not require referenceTime > timeInPast.
     * (That design uses while() do {} )
     * The external design does ensure referenceTime > timeInPast,
     * so we check that here, and use do {} while(), which otherwise might project too much.
     *
     * Strictly > (in the past)
     */
    myRequire( referenceTime > timeInPast);

    EpochTime workingProjection = timeInPast;

    /*
     *  Project forward by the period.
     *  Period usually 24 hours, but can be 12 hours, or any.
     */
    do {
        workingProjection += period;
    }
    while (workingProjection < referenceTime);
    // assert workingProjection >= referenceTime and workingProjection < referenceTime + Parameters::SunrisePeriod
    return workingProjection;
}


EpochTime TimeMath::projectTimeByPeriodToNearReferenceTime(const EpochTime timeInPast,
                                                        const EpochTime referenceTime,
                                                        const unsigned long int period,
                                                        const unsigned long int halfPeriod,
                                                        Interval &returnedInterval  // OUT
                                                        ) {
    myRequire( referenceTime > timeInPast);
    // actually timeInPast must be > period/2 in past

    Interval intervalToReferenceTime;
    EpochTime workingProjection = timeInPast;

    // Interval starts negative, advances toward zero
    intervalToReferenceTime =  workingProjection - referenceTime;
    myAssert(intervalToReferenceTime < 0);

    /*
     * while interval not in range [-period/2, period/2]
     *
     * while ( intervalToReferenceTime < 0 ) is NOT correct: it can overshoot.
     *
     * while ( not (intervalToReferenceTime.inRange( halfPeriod ) == RangeResult::InRange) ) is NOT robust,
     * can be infinite loop if parameters period and halfPeriod are incorrect.
     * More robust to loop until InRange or Greater.
     */
    while ( intervalToReferenceTime.inRange( halfPeriod ) == RangeResult::Lesser )
    {
        // project forward by period
        workingProjection += period;

        // update interval and test again
        intervalToReferenceTime =  workingProjection - referenceTime;
    }

    // Check overshoot when given incorrect parameters
    myAssert(intervalToReferenceTime.inRange( halfPeriod ) == RangeResult::InRange);

    returnedInterval = intervalToReferenceTime;
    return workingProjection;
}
