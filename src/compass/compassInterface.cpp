
/*
 * Low level functions of compass chip.
 */
#include "compassInterface.h"

#include "../bridge/bridge.h"






void CompassInterface::setLowPowerMode() {
    Bridge::setBits(static_cast<unsigned char>(CompassAddress::Control3),
                            0b100000);
}



void CompassInterface::setSingleConversionMode() {
    /*
     * lowest two bits are mode.
     * value 0b01 is mode:singleConversion.
     * There is no Bridge method to set a value in bit fields, so implementation is clear both bits, then set one.
     */
    Bridge::clearBits(static_cast<unsigned char>(CompassAddress::Control3),
                      0b11);
    // assert mode is continuous
    Bridge::setBits(static_cast<unsigned char>(CompassAddress::Control3),
                    0b1);
    // assert mode is single
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
    Bridge::read(static_cast<unsigned char>(CompassAddress::MagXYZBytes),
                       destination,
                       6);
}


bool CompassInterface::isReadable() {
    /*
     * Sane if part ID register reads correctly.
     * Read will return 0 if SPI or chip not working.
     * The chip does not need to be configured first.
     */
    unsigned char ID;

    bool success = Bridge::readByte(static_cast<unsigned char>(CompassAddress::Identifier), &ID);
    return (ID == LIS3MDLIdentifier);
}


unsigned char CompassInterface::readStatus() {
    unsigned char value;
    (void) Bridge::readByte(static_cast<unsigned char>(CompassAddress::Status), &value);
    // TODO handle error
    return value;
}

bool CompassInterface::isDataReady() {
    // zyxda bit set
    return (readStatus() & 0b1000 );
}
