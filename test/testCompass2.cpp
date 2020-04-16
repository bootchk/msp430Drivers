

#include "i2cTransaction.h"


#include "../src/compass/LIS3MDLRegisters.h"  // CompassAddress

#include "../src/compass/mangler.h"

#include "../src/busConfig.h"



// DriverLib
#include <pmm.h>  // unlock LPM5


void testCompass2()
{
    PMM_unlockLPM5();

    unsigned int result;


    // Test separate transactions

    // Configure to read the ID register
    I2CTransaction::writeByte(LIS3MDLAddress, CompassAddress::Identifier);
    // Read the configured register
    result = I2CTransaction::readByte();
    if (result == LIS3MDLIdentifier)
    {
        __no_operation();
    }

#ifdef TEMP
    // Test combined transaction of one byte

    // Address of ID register on compass
    result = I2CTransaction::receiveOneByte(CompassAddress::Identifier);
    if (result == LIS3MDLIdentifier)
    {
        __no_operation();
    }

    unsigned char magXYZ[6];
    int mag[3];

    I2CTransaction::receiveMultipleBytes(CompassAddress::MagXYZBytes,
                         magXYZ,
                         6);

    Mangler::convertRawBytesToIntMagneticData(magXYZ, mag);
    unsigned int heading = Mangler::convertThreeAxisIntMagneticDataToCompassHeading(mag);
#endif
}
