
#include "RTCInterface.h"

#include "../bridge/bridge.h"
#include "AB08xxRegisters.h"







void RTCInterface::writeAlarm(const RTCTime* alarm) {
#ifdef OLD
    Bridge::writeBuffer(static_cast<unsigned char> (RTCAddress::Alarm),
                        sizeof(RTCTime),
                        (unsigned char *) alarm);
#endif
    Bridge::write(static_cast<unsigned char> (RTCAddress::Alarm),
                 (unsigned char *) alarm,
                 sizeof(RTCTime));
    // assert alarm parameter is unchanged.
    // assert time was written to RTC
}







void RTCInterface::readAlarm(RTCTime* alarm) {
    // TODO
    Bridge::readBuffer(static_cast<unsigned char> (RTCAddress::Alarm),
                            sizeof(RTCTime),
                            (unsigned char *) alarm);
}


void RTCInterface::readTime(RTCTime* time) {
    Bridge::readBuffer(static_cast<unsigned char> (RTCAddress::Time),
                            sizeof(RTCTime),
                            (unsigned char *) time);
}
