
/*
 * Low level functions of compass chip.
 */
#include "compassInterface.h"

#include "../bridge/bridge.h"

#include "LIS3MDLRegisters.h"


void CompassInterface::readRawBytesOfMagneticData(unsigned char * destination) {
    /*
     * Compass is across bridge.
     * Mag data is many bytes.
     * Mag data starts at compass address
     */
    Bridge::readBuffer(static_cast<BridgedAddress> (CompassAddress::MagXYZBytes),
                       6,
                       destination);

}

bool CompassInterface::isReadable() {
    /*
     * Sane if upper byte of part ID reads as 08, from 08xx part family.
     * Read will return 0 if SPI or RTC not working.
     * The RTC does not need to be configured first.
     */
    unsigned char ID;

    ID = Bridge::read(static_cast<BridgedAddress> (CompassAddress::Identifier));
    return (ID == 0x08);
}
