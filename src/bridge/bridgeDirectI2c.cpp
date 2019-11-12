/*
 * Implementation of bridge without the Serial abstraction.
 * I.E. eliminates selection of slave, and choice of SPI vs I2C
 * Uses only I2C.
 *
 * To use this, exclude other implementations of bridge (.cpp files) from build.
 */

#include "bridge.h"

// implementation
#include "../i2c/transport/i2cTransport.h"

#include "../assert/myAssert.h"

// Configuration of I2C pullups
// File is external to library
#include <board.h>





/*
 * Read/write
 */
bool Bridge::write(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    //myRequire(isConfigured());
    return I2CTransport::write(registerAddress, buffer, count);
}


bool Bridge::read(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    // myRequire(isConfigured());
    return I2CTransport::read(registerAddress, buffer, count);
}


bool Bridge::writeByte(RegisterAddress registerAddress, unsigned char value) {
    //myRequire(isConfigured());

    return I2CTransport::write(registerAddress, value);

#ifdef VERIFY_BRIDGE_SINGLE_WRITES
    // reread to ensure slave has same value written (no glitch noise on wires)
    unsigned char finalValue = I2CTransport::read(registerAddress);
    myAssert(finalValue == value);
#endif
}


bool Bridge::writeByteWriteOnly(RegisterAddress registerAddress, unsigned char value) {
    return I2CTransport::write(registerAddress, value);
    // !!! Does not read back what was written
}


bool Bridge::readByte(RegisterAddress registerAddress, unsigned char* value) {
    //myRequire(isConfigured());
    return I2CTransport::read(registerAddress, value);
}







void Bridge::configureToSleepState() {
    I2CTransport::disable();
    I2CTransport::unconfigurePins();
}



void Bridge::configureMcuSide(bool isRWBitHighForRead) {
    // not require slave device ready since configuration is on the mcu side

    I2CTransport::initI2CPeripheral(0x69);
    // slave set
    // pins not configured
    // not enabled

#ifdef TEMP

    Cruft?  Need to decide on one API that allows setting data rate independently or not, that enables or not
    I2CTransport::setDataRate125kbps();

#ifdef    I2C_HAS_EXTERNAL_PULLUPS
    I2CTransport::configurePinsWithExternalPullups();
#elif defined(I2C_HAS_INTERNAL_PULLUPS)
    I2CTransport::configurePinsWithInternalPullups();
#else
#error "I2C pullups unspecified in board.h"
#endif

    I2CTransport::enable();
#endif

}


// TODO why duplicate
void Bridge::unconfigureMcuSide() {
    configureToSleepState();
}






/*
 * Convenience functions.
 * See I2C Device library, Github for comparable code.
 */
bool Bridge::setBits(RegisterAddress registerAddress, unsigned char mask) {

    unsigned char initialValue;
    if ( Bridge::readByte(registerAddress, &initialValue)) {
        return Bridge::writeByte(registerAddress, mask | initialValue );
    }
    else return false;
}

bool Bridge::clearBits(RegisterAddress registerAddress, unsigned char mask) {

    /*
     * initial value          11
     * mask                   01
     * ~mask                  10
     * initial value & ~mask  10
     */
    unsigned char initialValue;
    if ( Bridge::readByte(registerAddress, &initialValue) ) {
        return Bridge::writeByte(registerAddress, initialValue & ~mask );
    }
    else return false;
}






