
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

	static void configureToSleepState();
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



    /*
     * A full duplex transaction on the SPI bus.
     * Full duplex: Writes AND reads at the same time.
     * A value is written and value read at same 8-bit clocking.
     *
     * For a read at the app level:
     * The app should provide any value (bogus or dummy.)
     * The value written is usually ignored by the slave for a read.
     *
     * For a write at the app level:
     * and the value read may be ignored by the app.
     * (Is it the same value as was written?)
     *
     * Not asynch io; blocks.
     */
	static unsigned char transferDuplex(unsigned char);


    // Write data to register.
    static void write( const RegisterAddress registerAddress,
            unsigned char * const dataOut,
            const unsigned int count);

    // Read from register into buffer
    static void read( const RegisterAddress registerAddress,
            unsigned char * const bufferIn,
            const unsigned int count);




	// Is RX or TX interrupt flag set?
	static bool isInterrupt();
	static void clearInterrupt();

	static void selectSlave(unsigned int);
	static void deselectSlave();

	static RegisterAddress mangleRegisterAddress(ReadOrWrite readOrWrite, RegisterAddress address );
};
