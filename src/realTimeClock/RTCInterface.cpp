
#include "RTCInterface.h"

#include "../bridge/bridge.h"
#include "AB08xxRegisters.h"







void RTCInterface::writeAlarm(const RTCTime* alarm) {
    Bridge::write(static_cast<unsigned char> (RTCAddress::Alarm),
                 (unsigned char *) alarm,
                 sizeof(RTCTime));
    // assert alarm parameter is unchanged.
    // assert time was written to RTC
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
