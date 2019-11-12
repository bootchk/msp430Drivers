
/*
 * Interface to AB08xx RTC
 *
 * Results are RTCTime.
 * RTCTime is BCD encoded for AB08x5
 */

// type used to hold register sets from the AB08xx
#include "../time/rtcTime.h"


#define COUNT_BYTES_READ_WRITE_TO_ALARM 6





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
    static bool readTime(RTCTime*);

    /*
     * Write to the alarm registers (that we are using) of the RTC.
     * We configure the alarm to use only Hundredths,S,M,H,D,M
     *
     * The written RTCTime has a zero hundredths.
     * writeAlarm() does write hundredths to the alarm, with implications for correctness re shortest duration
     *
     * * RTCTime has a valid year.
     * The RTC does not have an alarm register for year, but does have a weekday (1-7) register
     * writeAlarm() does write year to the alarm but to the weekday register!!!
     * Since the match is not configured to use the weekday register, this is OK.
     */
    static bool writeAlarm(const RTCTime*);


    static bool readAlarm(RTCTime*);
};
