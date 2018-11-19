
#include "addressMangler.h"


bool RegisterAddressMangler::isReadHigh = true;

// TODO setter




unsigned char RegisterAddressMangler::mangle(BridgedAddress address, ReadOrWrite direction) {
    unsigned char result;

    if (isReadHigh and direction == ReadOrWrite::Read) {
        // bit set
        result = 128 | (unsigned char) address;
    }
    else {
        result = 127 & (unsigned char) address;
    }
    return result;
}
