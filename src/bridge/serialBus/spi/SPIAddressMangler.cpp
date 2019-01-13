
#include "SPIAddressMangler.h"


bool SPIRegisterAddressMangler::isRWBitHighForRead = true;





void SPIRegisterAddressMangler::configureRWBitHighForRead(bool yesNo) {
    SPIRegisterAddressMangler::isRWBitHighForRead = yesNo;
}




unsigned char SPIRegisterAddressMangler::mangle(RegisterAddress address, ReadOrWrite direction)
{
    unsigned char result;

    result = address;

    switch (direction) {
    case ReadOrWrite::Read :
        if (isRWBitHighForRead)
        {
            result = 128 | result; // bit set
        }
        else
        {
            result = 127 & result;
        }
        break;
    case ReadOrWrite::Write:
        if (isRWBitHighForRead)
        {
            result = 127 & result;
        }
        else
        {
            result = 128 | result;  // bit set
        }
    }

    // Temp test: set the multiple read byte, BIT6
    // result |= 64; // BIT6;

    return result;
}


