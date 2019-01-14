
#include "serial.h"

#include "../../driverConfig.h"




#ifdef SERIAL_IS_I2C
#define SERIAL_DEVICE_CLASS I2C
#include "i2c/i2c.h"
#endif

#ifdef SERIAL_IS_SPI
#define SERIAL_DEVICE_CLASS SPI
#include "spi/spi.h"
#endif





void Serial::begin(unsigned int slave, bool isRWBitHighForRead) {

    SERIAL_DEVICE_CLASS::disable(); // Can only configure when disabled.

	//Configure the mcu peripheral as master with parameters compatible with slave
	SERIAL_DEVICE_CLASS::configureMaster(isRWBitHighForRead);

	SERIAL_DEVICE_CLASS::selectSlave(slave);

	// TODO for i2c, don't enable until after set mode
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
unsigned char Serial::transferDuplex(ReadOrWrite direction,
                               unsigned char value) {
    // requires slave selected
    // requires configured
	return SERIAL_DEVICE_CLASS::transfer(direction, value);
}



void Serial::write(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    SERIAL_DEVICE_CLASS::write(registerAddress, buffer, count);
}
void Serial::read(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    SERIAL_DEVICE_CLASS::read(registerAddress, buffer, count);
}



void Serial::selectSlave(unsigned int slave) { SERIAL_DEVICE_CLASS::selectSlave(slave); }
void Serial::deselectSlave() { SERIAL_DEVICE_CLASS::deselectSlave(); }

RegisterAddress Serial::mangleRegisterAddress(ReadOrWrite readOrWrite, RegisterAddress address ) { return SERIAL_DEVICE_CLASS::mangleRegisterAddress(readOrWrite, address); }


