
/*
 * Interface to AB08xx RTC
 *
 * Results are RTCTime.
 * RTCTime is BCD encoded for AB08xx
 */

// the time type the AB08xx implements
#include "../time/rtcTime.h"
#include "AB08xxRegisters.h"



class RTCInterface {
private:
    //static void setAddress(RTCAddress subaddress);

public:
    static void readTime(RTCTime*);
    static void writeAlarm(const RTCTime*);
    static void readAlarm(RTCTime*);
};
