

#include "bridge.h"

/*
 * Crosses a Serial channel, typically  SPI or I2C
 */
#include "serialBus/serial.h"

#include "../assert/myAssert.h"

#include "../driverConfig.h"

//#include <src/bridge/SPIAddressMangler.h>




namespace {

unsigned int chosenDevice;

// state, for assertions that catch API errors
bool _isConfigured = false;


/*
 * Transfer many bytes over the bus.
 *
 * An SPI.transfer is duplex or bidirectional: both read and write in the same transfer.
 * On write, we ignore what is read from slave.
 *
 * Note that Serial does not implement any multi-byte transfers.
 * Instead, here we iterate over single byte transfers.
 */
void innerReadBuffer( unsigned char * bufferPtr, unsigned int size) {
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
	    /*
	     * Transfer as a read requires dummy byte here a distinctive pattern.
	     * Expect return value is from slave
	     */
		bufferPtr[i] = Serial::transfer(ReadOrWrite::Read,
		                                0b10101010);    // nonsense value
	}
}

void innerWriteBuffer( unsigned char * bufferPtr, unsigned int size) {
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
		// ignore returned read
		(void) Serial::transfer(ReadOrWrite::Write,
		                        bufferPtr[i]);
	}
}


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

    // (void) means discard values read during writes of address and value
	Serial::selectSlave(chosenDevice);

	RegisterAddress mangled;
	mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);
	(void) Serial::transfer(ReadOrWrite::Write, mangled);
	(void) Serial::transfer(ReadOrWrite::Write, value);

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
	// write registerAddress to bus, where address designates read the register
    RegisterAddress mangled;
    mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);
	(void) Serial::transfer(ReadOrWrite::Write, mangled);

	// read single byte of data
	result = Serial::transfer( ReadOrWrite::Read, 0 );
	Serial::deselectSlave();
	return result;
}




void Bridge::readBuffer(RegisterAddress registerAddress,
                        unsigned int length,
                        unsigned char * destination) {

    myRequire(isConfigured());

    Serial::selectSlave(chosenDevice);
    RegisterAddress mangled;
    mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);
    // write registerAddress
    (void) Serial::transfer(ReadOrWrite::Write, mangled);
    // transfer the data bytes
    innerReadBuffer(destination, length);
    Serial::deselectSlave();
}

void Bridge::writeBuffer(RegisterAddress registerAddress,
                        unsigned int length,
                        unsigned char * source) {

    myRequire(isConfigured());

    Serial::selectSlave(chosenDevice);
    RegisterAddress mangled;
    mangled = Serial::mangleRegisterAddress(ReadOrWrite::Write, registerAddress);
    // write registerAddress
    (void) Serial::transfer(ReadOrWrite::Write, mangled);
    // transfer the data bytes
    innerWriteBuffer(source, length);
    Serial::deselectSlave();
}
