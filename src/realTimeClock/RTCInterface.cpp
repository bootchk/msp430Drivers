
#include "RTCInterface.h"

#include "../bridge/bridge.h"
#include "AB08xxRegisters.h"

static BridgedAddress rtcAddress = {1,0};


void RTCInterface::setAddress(RTCAddress subaddress) {
    rtcAddress.subaddress = static_cast<unsigned char> (subaddress);
}


void RTCInterface::writeAlarm(const RTCTime* alarm) {
    setAddress(RTCAddress::Alarm);
    Bridge::writeBuffer(rtcAddress,
                        sizeof(RTCTime),
                        (unsigned char *) alarm);
    // assert alarm parameter is unchanged.
    // assert time was written to RTC
}


void RTCInterface::readAlarm(RTCTime* alarm) {
    setAddress(RTCAddress::Alarm);
    Bridge::readBuffer(rtcAddress,
                            sizeof(RTCTime),
                            (unsigned char *) alarm);
}


void RTCInterface::readTime(RTCTime* time) {
    setAddress(RTCAddress::Time);
    Bridge::writeBuffer(rtcAddress,
                            sizeof(RTCTime),
                            (unsigned char *) time);
}
