
/*
 * A serial interface using SPI
 *
 * Hides choice of serial kind of device
 * (both what kind of device it is, and what instance of the device.)
 *
 * Modeled after the Arduino/Energia SPI library,
 * or the Linux SPI library
 * or the RTOS SPI library.
 *
 * See the provided implementation.
 * You can provide other implementations.
 */


class Serial {
public:
	/*
	 * Enable device and configure data and clock pins for it.
	 * Does not configure chip (slave) select pin.
	 */
	static void begin();

	/*
	 * Writes AND reads at the same time.
	 * When the higher level semantic is "write",
	 * the caller should ignore the read value.
	 * When the higher level semantic is "read",
	 * the caller should provide any meaningless value to write.
	 *
	 * Not asynch io; blocks.
	 */
	static unsigned char transfer(unsigned char);

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
	static void selectSlave();
	static void deselectSlave();
};
