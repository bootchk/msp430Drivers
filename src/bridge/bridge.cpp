

#include "bridge.h"

/*
 * Crosses a Serial channel, typically  SPI or I2C
 */
#include "serialBus/serial.h"

#include "../assert/myAssert.h"

#include "../driverConfig.h"




namespace {

unsigned int chosenDevice;

// state, for assertions that catch API errors
bool _isConfigured = false;

} // namespace



bool Bridge::isConfigured() { return _isConfigured; }



/*
 * Three SPI pins are chosen by SPI library.
 *
 * Alternatively, the implementation could choose another SPI peripheral on the mcu,
 * using a different set of pins.
 *
 * See SPI::setModule() for choosing an alternate SPI peripheral.
 */
void Bridge::configureMcuSide(bool isRWBitHighForRead) {
	// not require isSPIReady() since configuration is on the mcu side

    /*
     * Modal choice of device
     */
    // TODO not implemented in lower layers to choose corresponding select line
    chosenDevice = 1;

	/*
	 * Enable serial device
	 * Configure a set of the mcu's GPIO pins for the serial peripheral/module
	 * e.g. a SPI module
	 */
	Serial::begin(chosenDevice, isRWBitHighForRead);

	_isConfigured = true;
}


void Bridge::unconfigureMcuSide() {
	Serial::end();
	_isConfigured = false;
	// assert all serial pins low power
}




void Bridge::writeByte(RegisterAddress registerAddress, unsigned char value) {
    myRequire(isConfigured());

	Serial::selectSlave(chosenDevice);

	//RegisterAddress mangled;
	//mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);

	unsigned char buffer[1];
	buffer[0] = value;
	Serial::write(registerAddress, buffer , 1);

	Serial::deselectSlave();

	// reread and return the value  that was written, so caller may ensure it was written correctly
	unsigned char finalValue = Bridge::readByte(registerAddress);
#ifdef VERIFY_BRIDGE_WRITES
	myAssert(finalValue == value);
#endif
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



unsigned char Bridge::readByte(RegisterAddress registerAddress) {
	unsigned char result;

	myRequire(isConfigured());

	Serial::selectSlave(chosenDevice);

    //RegisterAddress mangled;
    //mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);

	unsigned char buffer[1];

	Serial::read(registerAddress, buffer, 1);

	Serial::deselectSlave();
	return buffer[0];
}




void Bridge::readBuffer(RegisterAddress registerAddress,
                        unsigned int length,
                        unsigned char * destination) {

    myRequire(isConfigured());

    Serial::selectSlave(chosenDevice);

    //RegisterAddress mangled;
    //mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);

    Serial::read(registerAddress, destination, length);

    Serial::deselectSlave();
}


void Bridge::writeBuffer(RegisterAddress registerAddress,
                        unsigned int length,
                        unsigned char * source) {

    myRequire(isConfigured());

    Serial::selectSlave(chosenDevice);

    //RegisterAddress mangled;
    //mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);

    Serial::write(registerAddress, source, length);

    Serial::deselectSlave();
}
