
#include "addressMangler.h"


bool RegisterAddressMangler::isRWBitHighForRead = true;





void RegisterAddressMangler::configureRWBitHighForRead(bool yesNo) {
    RegisterAddressMangler::isRWBitHighForRead = yesNo;
}




unsigned char RegisterAddressMangler::mangle(BridgedAddress address, ReadOrWrite direction) {
    unsigned char result;

    if (isRWBitHighForRead and direction == ReadOrWrite::Read) {
        // bit set
        result = 128 | (unsigned char) address;
    }
    else {
        result = 127 & (unsigned char) address;
    }
    return result;
}


