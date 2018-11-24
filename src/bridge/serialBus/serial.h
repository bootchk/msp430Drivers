
#pragma once

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
	 * Writes AND reads at the same time.
	 * When the higher level semantic is "write",
	 * the caller should ignore the read value.
	 * When the higher level semantic is "read",
	 * the caller should provide any meaningless value to write.
	 *
	 * Not asynch io; blocks.
	 */
	static unsigned char transfer(
	        ReadOrWrite,        // direction of transfer on bus
	        unsigned char );    // value to transfer

	/*
	 * Disable device and configure pins for low power
	 * (MISO was input, make it output to insure no current draw.
	 * Since the remote device RTC should be driving it,
	 * the pin should not be floating and so would not draw current.
	 * So this might be unecessary.)
	 */
	static void end();

	/*
	 * Select slave on serial bus.
	 * This directs a transfer to one of many slave devices on bus.
	 */
	static void selectSlave(unsigned int slave);
	static void deselectSlave();
};
