
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

void RTCInterface::writeAlarm(const RTCTime* alarm) {
    Bridge::write(static_cast<unsigned char> (RTCAddress::Alarm),
                 (unsigned char *) alarm,
                 sizeof(RTCTime));
    // assert alarm parameter is unchanged.
    // assert alarm was written to RTC
}


void RTCInterface::readAlarm(RTCTime* alarm) {
    Bridge::read(static_cast<unsigned char> (RTCAddress::Alarm),
                       (unsigned char *) alarm,
                       sizeof(RTCTime));
}







void RTCInterface::readTime(RTCTime* time) {
    Bridge::read(static_cast<unsigned char> (RTCAddress::Time),
                       (unsigned char *) time,
                       sizeof(RTCTime));
}
