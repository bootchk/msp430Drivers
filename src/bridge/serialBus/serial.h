
#pragma once

#include "../bridge.h"   // RegisterAddress



/*
 * A serial interface using SPI or I2C.
 *
 * Hides choice of serial kind of device
 * (both what kind of device it is, and what instance of the device.)
 *
 * Modeled after the Arduino/Energia Wire library,
 * or the Linux SPI library
 * or the RTOS SPI library.
 *
 * See the provided implementation.
 * You can provide other implementations.
 *
 * Serial does not implement any multi-byte transfers.
 * Such are implemented in the layer above (Bridge).
 * (Even though some I2C libraries implement multi-byte transfers.
 *
 * Also, a session is designated read or write.
 * (Even though SPI is full duplex and a session can read and write at the same time.)
 * Also, even though a session is "read from slave", it requires reads and writes over the bus.
 */


enum class ReadOrWrite {
    Read,
    Write
};


class Serial {
public:
	/*
	 * Start a session.
	 *
	 * Configure and enable a serial device.
	 * Configure GPIO pins for it.
	 *
	 * Does not configure chip (slave) select pin. ????
	 */
	static void begin(unsigned int slave, bool isRWBitHighForRead);

    /*
     * Disable device and configure pins for low power
     * (MISO was input, make it output to insure no current draw.
     * Since the remote device RTC should be driving it,
     * the pin should not be floating and so would not draw current.
     * So this might be unecessary.)
     */
    static void end();



	/*
	 * Data moving methods.
	 */
	static void write(const RegisterAddress registerAddress,
	                  unsigned char * const buffer,
	                  const unsigned int count);
	static void read(const RegisterAddress registerAddress,
	                      unsigned char * const buffer,
	                      const unsigned int count);




	/*
	 * Select slave on serial bus.
	 * This directs a transfer to one of many slave devices on bus.
	 *
	 * SPI: drive dedicated signal high
	 * I2C: set slave address to be sent over bus
	 */
	static void selectSlave(unsigned int slave);
	static void deselectSlave();
	static bool isSlaveSelected();


	static RegisterAddress mangleRegisterAddress(ReadOrWrite, RegisterAddress address );
};
