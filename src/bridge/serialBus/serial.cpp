
#include "serial.h"

#include "../../driverConfig.h"
#include "../../assert/myAssert.h"



#ifdef SERIAL_IS_I2C
#define SERIAL_DEVICE_CLASS I2C
#include "i2c/i2c.h"
#endif

#ifdef SERIAL_IS_SPI
#define SERIAL_DEVICE_CLASS SPI
#include "spi/spi.h"
#endif






void Serial::configureToSleepState() {
    SERIAL_DEVICE_CLASS::configureToSleepState();
}




void Serial::begin(unsigned int slave, bool isRWBitHighForRead) {

    SERIAL_DEVICE_CLASS::disable(); // Can only configure when disabled.

	//Configure the mcu peripheral as master with parameters compatible with slave
	SERIAL_DEVICE_CLASS::configureMaster(isRWBitHighForRead);

	SERIAL_DEVICE_CLASS::selectSlave(slave);

	// TODO for i2c, don't enable until after set mode???
	SERIAL_DEVICE_CLASS::enable();

	// ensure ready for slave select and data moving operations
}


void Serial::end() {
	// ??? disabling saves power
	SERIAL_DEVICE_CLASS::disable();

	SERIAL_DEVICE_CLASS::unconfigureMaster();
}






void Serial::write(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    myRequire(isSlaveSelected());
    SERIAL_DEVICE_CLASS::write(registerAddress, buffer, count);
}


void Serial::read(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    myRequire(isSlaveSelected());
    myRequire(SERIAL_DEVICE_CLASS::isEnabled());
    SERIAL_DEVICE_CLASS::read(registerAddress, buffer, count);
}





namespace {
// local state, for assertions
bool _isSlaveSelected = false;
}

void Serial::selectSlave(unsigned int slave) {
    _isSlaveSelected = true;
    SERIAL_DEVICE_CLASS::selectSlave(slave);
}
void Serial::deselectSlave() {
    _isSlaveSelected = false;
    SERIAL_DEVICE_CLASS::deselectSlave();
}
bool Serial::isSlaveSelected() { return _isSlaveSelected; }






RegisterAddress Serial::mangleRegisterAddress(ReadOrWrite readOrWrite, RegisterAddress address ) { return SERIAL_DEVICE_CLASS::mangleRegisterAddress(readOrWrite, address); }


