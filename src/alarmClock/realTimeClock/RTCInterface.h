
/*
 * Interface to AB08xx RTC
 *
 * Results are RTCTime.
 * RTCTime is BCD encoded for AB08x5
 *
 * Low level, single operations mostly.
 */

// type used to hold register sets from the AB08xx
#include "../time/rtcTime.h"


#define COUNT_BYTES_READ_WRITE_TO_ALARM 6





class RTCInterface {
private:
    //static void setAddress(RTCAddress subaddress);

public:
    /*
     * We make time read-only.
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
     * writeAlarm() does write year to the alarm.
     * writeAlarm() does not write the weekday register, and the match is not configured to use the weekday register.
     */
    static bool writeAlarm(const RTCTime*);

    /*
     * Read alarm registers through month (omitting weekday.)
     * The Linux read_alarm also reads the status register and returns whether the alarm is pending.
     */
    static bool readAlarm(RTCTime*);

    /*
     * Write alarm and enable interrupt from it.
     */
    static bool setAlarm(const RTCTime*);

    /*
     * Arrange that alarm generates interrupt signal internally.
     * Also must arrange that signal reaches a pin of the RTC.
     */
    static bool enableAlarmIRQ();





    /*
     * Config time (once) operations.
     */

    /*
     * Configure RTC so that match(counter, alarm) is on the entire alarm (i.e. once per year).
     * Alternatively, the match can partial on the alarm, and generate AIRQ repeatedly e.g. once per second.
     * We don't use this to disable alarm, we leave it set.
     * "Once per year" does not mean the alarm can't be changed, and alarm go off once per setAlarm().
     */
    static bool configureAlarmMatchPerYear();

    static bool configure24HourMode();
};
