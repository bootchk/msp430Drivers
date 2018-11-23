
/*
 * Low level functions of compass chip.
 */
#include "compassInterface.h"

#include "../bridge/bridge.h"



static BridgedAddress compassAddress = {1,0};


void CompassInterface::setCompassAddress(CompassAddress subaddress) {
    compassAddress.subaddress = static_cast<unsigned char>(subaddress);
}




void CompassInterface::setLowPowerMode() {
    setCompassAddress(CompassAddress::Control3);
    Bridge::setBits(compassAddress,
                            0b100000);
}



void CompassInterface::setSingleConversionMode() {
    /*
     * lowest two bits are mode.
     * value 0b01 is mode:singleConversion.
     * There is no Bridge method to set a value in bit fields, so implementation is clear both bits, then set one.
     */
    setCompassAddress(CompassAddress::Control3);
    Bridge::clearBits(compassAddress, 0b11);
    // assert mode is continuous
    Bridge::setBits(compassAddress, 0b1);
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
    setCompassAddress(CompassAddress::MagXYZBytes);
    Bridge::readBuffer(compassAddress,
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

    setCompassAddress(CompassAddress::Identifier);

    ID = Bridge::read(compassAddress);
    return (ID == LIS3MDLIdentifier);
}


unsigned char CompassInterface::readStatus() {
    setCompassAddress(CompassAddress::Status);

    return Bridge::read(compassAddress);
}

bool CompassInterface::isDataReady() {
    // zyxda bit set
    return (readStatus() & 0b1000 );
}
