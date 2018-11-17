
#include "serial.h"

#include "spi.h"

#include "../../pinFunction/spiPins.h"



void Serial::begin() {
	/*
	 * Only configures 3 of the 4 pins (not the Slave Select pin):
	 * MOSI, MISO, SCLK
	 *
	 * TI Energia document doesn't say this configure MISO ???
	 */
	/*
	 * Configure the mcu SPI peripheral with parameters of rtc chip's SPI
	 */
	SPI::disable();	// Can only configure when disabled.
	SPI::configureMaster();
	SPI::enable();

	// ensure ready for transfer()
}


void Serial::end() {
	// ??? disabling saves power
	SPI::disable();

	SPI::unconfigureMaster();
}


unsigned char Serial::transfer(unsigned char value) {
    // requires slave selected
    // requires configured
	return SPI::transfer(value);
}

// Delegate to SPIPins
void Serial::selectSlave() { SPIPins::selectSlave(); }
void Serial::deselectSlave() { SPIPins::deselectSlave(); }



#ifdef NOT_USED

Cruft from Energia SPI implementation

Serial::setBitOrder(MSBFIRST);
Serial::setDataMode(SPI_MODE0);
Serial::setClockDivider(SPI_CLOCK_DIV128);

#endif
