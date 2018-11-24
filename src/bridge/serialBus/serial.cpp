
#include "serial.h"

// Change the implementation between two choices
//#define SERIAL_DEVICE_CLASS SPI
//#include "spi/spi.h"
#define SERIAL_DEVICE_CLASS I2C
#include "i2c/i2c.h"




void Serial::begin(unsigned int slave, bool isRWBitHighForRead) {

    SERIAL_DEVICE_CLASS::disable(); // Can only configure when disabled.

	//Configure the mcu peripheral as master with parameters compatible with slave
	SERIAL_DEVICE_CLASS::configureMaster(isRWBitHighForRead);

	SERIAL_DEVICE_CLASS::selectSlave(slave);
	SERIAL_DEVICE_CLASS::enable();

	// ensure ready for transfer()
}


void Serial::end() {
	// ??? disabling saves power
	SERIAL_DEVICE_CLASS::disable();

	SERIAL_DEVICE_CLASS::unconfigureMaster();
}


/*
 * Some implementations (SPI) may ignore the direction
 */
unsigned char Serial::transfer(ReadOrWrite direction,
                               unsigned char value) {
    // requires slave selected
    // requires configured
	return SERIAL_DEVICE_CLASS::transfer(direction, value);
}


void Serial::selectSlave(unsigned int slave) { SERIAL_DEVICE_CLASS::selectSlave(slave); }
void Serial::deselectSlave() { SERIAL_DEVICE_CLASS::deselectSlave(); }



#ifdef NOT_USED

Cruft from Energia SPI implementation

Serial::setBitOrder(MSBFIRST);
Serial::setDataMode(SPI_MODE0);
Serial::setClockDivider(SPI_CLOCK_DIV128);

#endif
