
#include "RTCInterface.h"

#include "AB08xxRegisters.h"

#include "../../bridge/bridge.h"







/*
 * RTCTime is an amalgam.
 * When written to the alarm registers,
 * it actually writes the YearOfCentury field into the alarm "Weekday" register.
 * It will read back correctly.
 * Since the configured RTC alarm mode is ignoring the "Weekday" register, this works.
 */

bool RTCInterface::writeAlarm(const RTCTime* alarm) {
    return Bridge::write(static_cast<unsigned char> (RTCAddress::Alarm),
                 (unsigned char *) alarm,
                 COUNT_BYTES_READ_WRITE_TO_ALARM);  // OLD sizeof(RTCTime));
    // assert alarm parameter is unchanged.
    // assert alarm was written to RTC
}


bool RTCInterface::readAlarm(RTCTime* alarm) {
    return Bridge::read(static_cast<unsigned char> (RTCAddress::Alarm),
                       (unsigned char *) alarm,
                       COUNT_BYTES_READ_WRITE_TO_ALARM);
}







bool RTCInterface::readTime(RTCTime* time) {
    return Bridge::read(static_cast<unsigned char> (RTCAddress::Time),
                       (unsigned char *) time,
                       sizeof(RTCTime));
}
