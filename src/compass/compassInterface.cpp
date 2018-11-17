
/*
 * Low level functions of compass chip.
 */
#include "compassInterface.h"

#include "../bridge/bridge.h"

#include "LIS3MDLRegisters.h"


void CompassInterface::setLowPowerMode() {
    Bridge::setBits(static_cast<BridgedAddress> (CompassAddress::Control3),
                            0b100000);
}



void CompassInterface::setSingleConversionMode() {
    /*
     * lowest two bits are mode.
     * value 0b01 is mode:singleConversion.
     * There is no Bridge method to set a value in bit fields, so implementation is clear both bits, then set one.
     */
    Bridge::clearBits(static_cast<BridgedAddress> (CompassAddress::Control3),
                    0b11);
    Bridge::setBits(static_cast<BridgedAddress> (CompassAddress::Control3),
                        0b1);
}


void CompassInterface::readRawBytesOfMagneticData(unsigned char * destination) {
    /*
     * Compass is across bridge.
     * Mag data is many bytes.
     * Mag data starts at address of XYZ registers in compass chip.
     * Transfer data from compass chip to local buffer.
     */
    // TODO data ready
    // single transfer mode
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



bool CompassInterface::isDataReady() {
    // zyxda bit set
    return (Bridge::read(static_cast<BridgedAddress> (CompassAddress::Status)) & 0b1000 );
}
