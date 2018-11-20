
/*
 * Interface to AB08xx RTC
 *
 * Results are RTCTime.
 * RTCTime is BCD encoded for AB08xx
 */

// the time type the AB08xx implements
#include "../time/rtcTime.h"



class RTCInterface {
public:
    static void readTime(RTCTime*);
    static void writeAlarm(const RTCTime*);
    static void readAlarm(RTCTime*);
};
