/*
 * Implementation of bridge without the Serial abstraction.
 * I.E. eliminates selection of slave, and choice of SPI vs I2C
 * Uses only I2C.
 *
 * To use this, exclude other implementations of bridge (.cpp files) from build.
 */

#include "bridge.h"

// implementation
#include "serialBus/i2c/i2cTransport.h"

#include "../assert/myAssert.h"



void Bridge::write(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    //myRequire(isConfigured());
    I2CTransport::write(registerAddress, buffer, count);
}


void Bridge::read(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    // myRequire(isConfigured());
    I2CTransport::read(registerAddress, buffer, count);
}







void Bridge::configureToSleepState() {
    I2CTransport::disable();
    I2CTransport::unconfigurePins();
}







void Bridge::configureMcuSide(bool isRWBitHighForRead) {
    // not require device ready since configuration is on the mcu side

    I2CTransport::initI2CPeripheral(0x69);
    // slave set
    // pins not configured
    // not enabled

    I2CTransport::setDataRate125kbps();

    I2CTransport::configurePinsWithExternalPullups();

    I2CTransport::enable();
}


// TODO why duplicate
void Bridge::unconfigureMcuSide() {
    configureToSleepState();
}






/*
 * See I2C Device library, Github for comparable code.
 */
void Bridge::setBits(RegisterAddress registerAddress, unsigned char mask) {

    unsigned char initialValue = Bridge::readByte(registerAddress);

    Bridge::writeByte(registerAddress, mask | initialValue );
}

void Bridge::clearBits(RegisterAddress registerAddress, unsigned char mask) {

    /*
     * initial value          11
     * mask                   01
     * ~mask                  10
     * initial value & ~mask  10
     */
    unsigned char initialValue = Bridge::readByte(registerAddress);

    Bridge::writeByte(registerAddress, initialValue & ~mask );
}





void Bridge::writeByte(RegisterAddress registerAddress, unsigned char value) {
    //myRequire(isConfigured());

    I2CTransport::write(registerAddress, value);

#ifdef VERIFY_BRIDGE_WRITES
    // reread to ensure slave has same value written (no glitch noise on wires)
    unsigned char finalValue = Bridge::readByte(registerAddress);
    myAssert(finalValue == value);
#endif
}


unsigned char Bridge::readByte(RegisterAddress registerAddress) {

    //myRequire(isConfigured());
    return I2CTransport::read(registerAddress);
}
