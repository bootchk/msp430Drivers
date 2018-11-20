
#include "RTCInterface.h"

#include "../bridge/bridge.h"
#include "AB08xxRegisters.h"




void RTCInterface::writeAlarm(const RTCTime* alarm) {

    Bridge::writeBuffer(static_cast<BridgedAddress>(RTCAddress::Alarm),
                        sizeof(RTCTime),
                        (unsigned char *) alarm);
    // assert alarm parameter is unchanged.
    // assert time was written to RTC
}

void RTCInterface::readAlarm(RTCTime* alarm) {
    Bridge::readBuffer(static_cast<BridgedAddress>(RTCAddress::Alarm),
                            sizeof(RTCTime),
                            (unsigned char *) alarm);
}


void RTCInterface::readTime(RTCTime* time) {
    Bridge::writeBuffer(static_cast<BridgedAddress>(RTCAddress::Time),
                            sizeof(RTCTime),
                            (unsigned char *) time);
}
