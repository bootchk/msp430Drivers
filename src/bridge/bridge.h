
#pragma once


/*
 * Abstracts access across a bridge.
 * A bridge across a serial channel.
 *
 * Hides slave select.
 *
 * Hides multi-byte transfers.
 *
 * !!! The class is pure, with no instances.
 * Only one bridge can be used at a time.
 * E.G. Can't use I2C and SPI bus at same time.
 * However, they can be used consecutively.
 *
 * A bridge is a bus having possibly many slaves.
 * A bridge is modal on the selected slave.
 * You choose a single slave when Bridge you configure.
 * You can't use a different slave without reconfiguring Bridge.
 * TODO parameter for slave address
 *
 * The Bridge class is a singleton.
 *
 * A registerAddress on remote device is an unsigned byte.
 * First you configure Bridge for a device (which on some busses has a slaveAddress.)
 * Then you address a registerAddress (a subaddress?) on the device.
 */

/*
 * Lower level hiding:
 *
 * Serial hides choice of two different serial channels: SPI or I2C
 * Change channel by substituting a different implementation.
 *
 * Hides any quirks of the remote device protocol:
 * - the upper bit(s) of register address have special meaning.
 * - send registerAddress, then values
 *
 * Hides handshaking: which is not necessary when slave is always ready for serial communication.
 * More handshaking is necessary when there are many masters.
 */

typedef unsigned char RegisterAddress;


class Bridge {
public:
    static bool isConfigured();

	/*
	 * Configure mcu side of interface.
	 * Interface comprises:
	 * - peripheral of mcu (master)
	 * - GPIO pins
	 * -- data and clock pins
	 * -- slave select pins (for some bridges)
	 */
	static void configureMcuSide(bool isRWBitHighForRead);

	/*
	 * Configure Bridge pins to low power state.
	 *
	 * Energia docs for SPI lib say this does not change the GPIO configuration,
	 * only disable the SPI function on them (disables the driving device.)
	 * Since MOSI and SClk are already outputs, they are low power.
	 * MISO is an input, but driven by the RTC so it should be low power.
	 * The documents do not say this changes MISO from input to output.
	 *
	 * This does not affect the SlaveSelect pin.
	 * But it is an output, also low power.
	 */
	static void unconfigureMcuSide();



	/*
	 * Read/write many consecutive bytes
	 */
	static void read(const RegisterAddress registerAddress,
	                  unsigned char * const buffer,
	                  const unsigned int count);
	static void write(const RegisterAddress registerAddress,
	                      unsigned char * const buffer,
	                      const unsigned int count);


	/*
	 * Convenience (provides buffer for more general write())
	 * Write one byte to remote register at RegisterAddress registerAddress.
	 *
	 * Assertions can ensure written register contains value.
	 * Said assertions read back the register.
	 * This is not appropriate when a read will clear a register
	 * (But there are no such registers on this RTC chip.)
	 */
	static unsigned char readByte(RegisterAddress registerAddress);
	static void writeByte(RegisterAddress registerAddress, unsigned char value);




	/*
	 * Convenience functions.
	 * There are typically read (to know what is already set)
	 * then write (with the new value)
	 */
	/*
	 * Sets bits given by mask to remote register at registerAddress.
	 * Ensures that masked bits of register are set.
	 * Ensures all other bits (not masked) retain their values.
	 *
	 * Not general purpose: more generally, you would pass a mask and a value for the masked bits.
	 */
	static void setBits(RegisterAddress registerAddress, unsigned char mask);

	static void clearBits(RegisterAddress registerAddress, unsigned char mask);

};
