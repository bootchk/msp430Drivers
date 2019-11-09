
/*
 * Implements Serial API for I2C bus.
 *
 * Hides choice of I2C instance.
 *
 * See specifications at Serial.
 */

#include "../bridge/serialBus/serial.h"  // ReadOrWrite

#include "../bridge/bridge.h"   // RegisterAddress


class I2C {
private:
	static void configureMasterDevice();

	static void configureMasterPins();
	static void unconfigureMasterPins();

public:
	static void configureToSleepState();
	static bool isInSleepState();

	static void enable();
	static void disable();
	static bool isEnabled();

	/*
	 * Configure SPI.
	 * When there are multiple slaves, call this before switching to another slave.
	 * When there is only one slave, can call this once during each booted session.
	 *
	 * TODO I2C comments
	 */
	static void configureMaster(bool isRWBitHighForRead);

	/*
	 * Leave device configured, but configure pins for low power.
	 * After this, no SPI methods should be used until configured again.
	 */
	static void unconfigureMaster();





	static void write(const RegisterAddress registerAddress,
	                      unsigned char * const buffer,
	                      const unsigned int count);
	static void read(const RegisterAddress registerAddress,
	                          unsigned char * const buffer,
	                          const unsigned int count);


	// Is RX or TX interrupt flag set?
	static bool isInterrupt();
	static void clearInterrupt();

	static void selectSlave(unsigned int);
	static void deselectSlave();


	/*
	 * Generic, used for SPI???
	 */
	static unsigned char transfer(ReadOrWrite,  unsigned char);

	static RegisterAddress mangleRegisterAddress(ReadOrWrite readOrWrite, RegisterAddress address );
};
