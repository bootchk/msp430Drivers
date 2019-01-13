
/*
 * Implements API Serial for SPI bus.
 *
 * Hides choice of SPI master peripheral instance, and other implementation design choices.
 * (E.G. eUSCI_A versus eUSCI_B using DriverLib)
 *
 * API documented at Serial.h.
 */

#include "../serial.h"  // ReadOrWrite




class SPI {
private:
	static void configureMasterDevice();

	static void configureMasterPins();
	static void unconfigureMasterPins();

public:
	static void enable();
	static void disable();
	static bool isEnabled();


	/*
	 * Configure SPI.
	 * When there are multiple slaves, call this before switching to another slave.
	 * When there is only one slave, can call this once during each booted session.
	 */
	static void configureMaster(bool isRWBitHighForRead);

	/*
	 * Leave device configured, but configure pins for low power.
	 * After this, no SPI methods should be used until configured again.
	 */
	static void unconfigureMaster();


	static unsigned char transfer(ReadOrWrite, unsigned char);

	// Is RX or TX interrupt flag set?
	static bool isInterrupt();
	static void clearInterrupt();

	static void selectSlave(unsigned int);
	static void deselectSlave();
};
