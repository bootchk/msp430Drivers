
/*
 * Implements a SPI device interface.
 * Hides choice of SPI instance.
 */

/*
 * See specifications at Serial.
 */
class SPI {
private:
	static void configureMasterDevice();

	static void configureMasterPins();
	static void unconfigureMasterPins();

public:
	static void enable();
	static void disable();
	static bool isEnabled();
	static void configureMaster();

	/*
	 * Leave device config, but configure pins for low power.
	 */
	static void unconfigureMaster();

	static unsigned char transfer(unsigned char);

	// Is RX or TX interrupt flag set?
	static bool isInterrupt();
	static void clearInterrupt();
};
