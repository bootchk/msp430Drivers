
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
     * Sane if part ID register reads correctly.
     * Read will return 0 if SPI or chip not working.
     * The chip does not need to be configured first.
     */
    unsigned char ID;

    ID = Bridge::read(static_cast<BridgedAddress> (CompassAddress::Identifier));
    return (ID == LIS3MDLIdentifier);
}
