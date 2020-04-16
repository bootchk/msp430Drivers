/*
 * Implentation using own driver
 */


#include "i2c.h"

// Uses a layer that hides
#include "transport/i2cTransport.h"

#include "i2cPeripheral.h"


#include "../assert/myAssert.h"






void  I2C::configureToSleepState() {
    /*
     * I2C CLK and SDA must be held high during idle (while mcu is asleep.)
     * Self mcu is the master.
     * If there are external pullups, pins can be floating inputs, without pullups.
     * If mcu is providing pullup, pins should be held as outputs, value high.
     */
    I2CTransport::unconfigurePins();
}


bool I2C::isInSleepState() {
    // sleep state is master disabled and I2C pins configured outputs high
    return ( (not isEnabled()) and I2CTransport::isUnconfigurePins());
}




/*
 * For I2C, selection is modal (does not need to be selected for each read/write.)
 *
 * For I2C, since selection is modal in the driver, deselection only happens when you select a new slave.
 * So deselection implementation is impotent.
 *
 * For generality with SPI,selectSlave and deselectSlave can be called more often.
 */

void I2C::selectSlave(unsigned int slaveOrdinal) {
    /*
     * TODO
     *
     * Now, slave is selected once when master driver is configured.
     *
     * Future: change the slave address in the driver.
     */
    // TODO set slave address from a table.
    // slave state owned by self
}

void I2C::deselectSlave() {
    // Does nothing.
    // Optionally, change modal slave address in driver to a null value so future calls will fail.
}





void I2C::enable() {  I2CTransport::enable(); }
void I2C::disable() { I2CTransport::disable(); }
bool I2C::isEnabled() { return I2CTransport::isEnabled(); }





void I2C::configureMaster(unsigned int slaveAddress, bool isRWBitHighForRead) {
    // require disabled to config
    myRequire( not isEnabled() );

    I2CTransport::configurePinsWithInternalPullups();

    I2CTransport::initI2CPeripheral(slaveAddress);

    I2CPeripheral::clearInterruptFlags();
}




void I2C::write(const RegisterAddress registerAddress,
                          unsigned char * const buffer,
                          const unsigned int count) {
    I2CTransport::write(registerAddress, buffer, count);
}

void I2C::read(const RegisterAddress registerAddress,
                          unsigned char * const buffer,
                          const unsigned int count) {
#ifdef EXTRA_I2C_INIT
    // TEMP test reconfigure before every read, for the hanging initialTransition issue
    I2C::disable();
    I2C::configureMaster(false);
    I2C::enable();
#endif

    I2CTransport::read(registerAddress, buffer, count);
}



void I2C::unconfigureMaster() {
    /*
     * Don't need to unconfigure I2C master (eUSCI), since disabled, is reset.
     * Only unconfigure pins.
     */
    I2CTransport::unconfigurePins();
}


/*
 * I2C bus does not require mangling of address to indicate read/write.
 */
RegisterAddress I2C::mangleRegisterAddress(ReadOrWrite readOrWrite, RegisterAddress address ) {
    return address;
}
