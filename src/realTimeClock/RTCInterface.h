
/*
 * Interface to AB08xx RTC
 *
 * Results are RTCTime.
 * RTCTime is BCD encoded for AB08x5
 */

// type used to hold register sets from the AB08xx
#include "../time/rtcTime.h"



class RTCInterface {
private:
    //static void setAddress(RTCAddress subaddress);

public:
    /*
     * Time is read-only.
     * RTC chip allows it to be written, but we choose not to.
     * We choose to not sync the clock with any other clock,
     * so we don't need to write the time.
     * Also, we don't store any times long term,
     * so we never need to write time to restore clock that has stopped.
     */
    static void readTime(RTCTime*);

    static void writeAlarm(const RTCTime*);
    static void readAlarm(RTCTime*);
};
