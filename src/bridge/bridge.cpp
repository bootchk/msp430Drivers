
#include "bridge.h"

// The serial bus that the bridge crosses
#include "serialBus/serial.h"

#include "addressMangler.h"

//#include "../../debug/myAssert.h"

/*
 * Uses a Serial channel, typically  SPI or I2C
 */




namespace {

/*
 * Transfer many bytes over SPI.
 *
 * The SPI.transfer is duplex or bidirectional: both read and write in the same transfer.
 * On write, we ignore what is read from slave.
 */
void innerReadBuffer( unsigned char * bufferPtr, unsigned int size) {
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
	    /*
	     * Transfer as a read requires dummy byte here a distinctive pattern.
	     * Expect return value is from slave
	     */
		bufferPtr[i] = Serial::transfer(0b10101010);
	}
}

void innerWriteBuffer( unsigned char * bufferPtr, unsigned int size) {
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
		// ignore returned read
		(void) Serial::transfer(bufferPtr[i]);
	}
}


} // namespace




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
	 * Enable serial device
	 * Configure a set of the mcu's GPIO pins for the serial peripheral/module
	 * e.g. a SPI module
	 */
	/*
	 * we don't need to specify SS pin on Energia?
	 * Docs for begin() says is configures default SS pin in the SPI library.
	 * But code seems to indicate it doesn't.
	 * Although there exists an overloaded transfer(SSpin, value) method apparently not implemented on Energia.
	 */
	Serial::begin(isRWBitHighForRead);
}


void Bridge::unconfigureMcuSide() {
	Serial::end();
	// assert all serial pins low power
}




void Bridge::write(BridgedAddress address, unsigned char value) {
	// require mcu Serial interface configured

    // discard values read during writes of address and value
	Serial::selectSlave();
	(void) Serial::transfer(RegisterAddressMangler::mangle(address, ReadOrWrite::Write));
	(void) Serial::transfer( value);

	Serial::deselectSlave();

	unsigned char finalValue = Bridge::read(address);
	//myAssert(finalValue == value);
}


/*
 * See I2C Device library, Github for comparable code.
 */
void Bridge::setBits(BridgedAddress address, unsigned char mask) {

    unsigned char initialValue = Bridge::read(address);

    Bridge::write(address, mask | initialValue );
}

void Bridge::clearBits(BridgedAddress address, unsigned char mask) {

    /*
     * initial value          11
     * mask                   01
     * ~mask                  10
     * initial value & ~mask  10
     */
    unsigned char initialValue = Bridge::read(address);

    Bridge::write(address, initialValue & ~mask );
}



unsigned char Bridge::read(BridgedAddress address) {
	// require mcu Serial interface configured

	unsigned char result;

	Serial::selectSlave();
	// transfer address
	(void) Serial::transfer(RegisterAddressMangler::mangle(address, ReadOrWrite::Read));
	// transfer single byte of data
	result = Serial::transfer( 0 );
	Serial::deselectSlave();
	return result;
}


void Bridge::readBuffer(BridgedAddress address,
                        unsigned int length,
                        unsigned char * destination) {

    Serial::selectSlave();
    // transfer the address
    (void) Serial::transfer(RegisterAddressMangler::mangle(address, ReadOrWrite::Read));
    // transfer the data bytes
    innerReadBuffer(destination, length);
    Serial::deselectSlave();
}

void Bridge::writeBuffer(BridgedAddress address,
                        unsigned int length,
                        unsigned char * source) {

    Serial::selectSlave();
    // transfer the address
    (void) Serial::transfer(RegisterAddressMangler::mangle(address, ReadOrWrite::Write));
    // transfer the data bytes
    innerWriteBuffer(source, length);
    Serial::deselectSlave();
}
