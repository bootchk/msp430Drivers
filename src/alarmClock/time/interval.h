#pragma once

/*
 * Difference of two EpochTimes, without constraint on order of EpochTimes operands.
 * Interval is signed, can be negative.
 *
 * Absolute value is a Duration.
 *
 * No enforcement of overflow:
 * my app deals in small time scale (a few days)
 * compared to what EpochTime will represent (centuries)
 *
 * Since a day is > 86k seconds, this must be a long to hold more than a few days difference.
 */

#include "../../assert/myAssert.h"


enum class RangeResult {
    Lesser,
    InRange,
    Greater
};


//typedef long int Interval;

struct Interval {
    long int plusOrMinusSeconds;


    /*
     * default + parameterized constructor
     */
    // No guard against interpreting very large unsigned as a signed negative number
    Interval(signed long initialValue = 0) : plusOrMinusSeconds(initialValue)   { }
    // Interval(int plusOrMinusSeconds = 0) : plusOrMinusSeconds(plusOrMinusSeconds)   { }

    /*
     * Addition operator. Doesn't modify object, therefore const.
     *
     * Standard signed integer arithmetic.
     * No guard against overflow
     */
    Interval operator+(const Interval& a) const
    {
         return Interval(plusOrMinusSeconds + a.plusOrMinusSeconds);
    }


    /*
     * Signed integer division
     */
    Interval operator/(const unsigned int& a) const
    {
         return Interval(plusOrMinusSeconds / a);
    }

    // Negation operator
    Interval operator-() const
    {
         return -plusOrMinusSeconds;
    }



#ifdef NOTUSED
    /*
     * Operator on Interval.
     * Add an EpochTime to an Interval, returning EpochTime
     */
    // TODO really need operator on EpochTime
    unsigned long int operator+(const unsigned long int& a) const
    {
        signed long int intermediateResult;

        // Convert unsigned EpochTime to signed (overflow possible for huge epoch time)
        intermediateResult = a;

        intermediateResult += plusOrMinusSeconds;
        myAssert(intermediateResult >0);

        // Convert back to unsigned long,
        return intermediateResult;
    }
#endif


    // Comparison between Intervals
    bool operator>(const Interval& a) const
    {
         return (plusOrMinusSeconds > a.plusOrMinusSeconds);
    }

    // Comparison to signed int
    bool operator>=(const signed int & a) const
    {
        return (plusOrMinusSeconds >= a);
    }

    bool operator<(const signed int & a) const
    {
        return (plusOrMinusSeconds < a);
    }


    /*
     * Is in range [-range, range} ?
     */
    /*
     * TODO The length of the range is 2*range plus 1 unless one of the operators is <= or >= ?
     */
    RangeResult inRange(const unsigned long & range) const
    {
        RangeResult result;

        /*
         * plusOrMinusSeconds is signed while range is unsigned
         * MUST not compare signed to unsigned, since comparison is unsigned if either operand is unsigned after promotion.
         * The unsigned long type cannot be promoted to unsigned (some values not representable.)
         *
         * TODO optimize by passing unsigned int range (not long) since then it is promotable to signed.
         */
        signed long lowerBound = -range;
        signed long upperBound = range;

        if (plusOrMinusSeconds < lowerBound) result = RangeResult::Lesser;
        else if (plusOrMinusSeconds > upperBound) result = RangeResult::Greater;
        else result = RangeResult::InRange;

        return result;
    }
} ;
