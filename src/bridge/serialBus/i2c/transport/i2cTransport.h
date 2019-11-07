
#include "../../../bridge.h"  // type RegisterAddress



// Alternative implementations for link layer
#define USE_DRIVERLIB_FOR_LINK
//#define USE_DIRECT_FOR_LINK
//#define USE_STATE_MACHINE_FOR_LINK

/*
 * I2C transport
 *
 * A layer on top of a I2C link from master to slave.
 *
 * Write and Read functions.
 * A Write in transport layer is two Sends in the link layer (actually one long send.)
 * A Read in transport layer is a Send and a Receive in the link layer (with a restart between.)
 *
 * The first Send is a Command byte (a register address.)
 * For a write, the second Send in the link layer is a Packet of one or more data bytes to be written to the register.
 * For a read, the second Receive in the link layer is a Packet of one or more data bytes read from the register.
 */

/*
 * Driver for transport over I2C.
 * Also known as "combined transactions"
 *
 * Master sends command (register addresses and other bits)
 * and then either master sends data (to the register address)
 * or master receives data from slave (from the register address)
 *
 * The latter case requires a "restart", to turn the link around.
 *
 * Each call blocks until complete.
 * The block comprises MCU sleeping in LPM0, with interrupts.
 *
 * I don't think this is robust in the presence of other interrupts.
 * In other words, using I2C transport should be the only thing going on in the system.
 * Other interrupts (such as timers) may disrupt I2C transport,
 * or I2C transport may delay servicing of other interrupts.
 */

/*
 * Parameters to transport calls:
 * -slave device address.
 * -register (command) to send to the slave.
 * -buffer where packet  is sent or received
 * -length of packet.
 */


/*
 * Algebra of calls:  initGPIO, unlockLPM5, disable, initI2CPeripheral, enable, [write, read]*, disable, enable, ...
 *
 * The I2C peripheral is always left in the idle state.
 * You don't need to do anything special to make it low power,
 * except to rip its GPIO pins away (make them general purpose instead of module).
 * The peripheral won't request the SMCLK then?
 * ???? Or do I need to reset peripheral???

 */

/*
 * Implementation uses interrupts and USCI_B0_ISR.
 * I2C should be the only source of interrupts during use of this class,
 * since I haven't tested how other interrupts and delays might affect the ISR for I2C.
 */

class I2CTransport {


public:
    static void configurePinsWithExternalPullups();
    static void configurePinsWithInternalPullups();
    // Are pins configured for module function?  Says nothing about pullups.
    static bool isConfiguredPinsForModule();

    static void unconfigurePins();
    static bool isUnconfigurePins();

    /*
     * Prepare for use with a particular slave.
     * There is no unconfigure, but unconfigurePins uninitializes.
     *
     * Only 7 bits of slaveAddress are valid.
     *
     * Does set data rate.
     *
     * Does not configure pins, or enable.
     */
    static void initI2CPeripheral(unsigned int slaveAddress);

    static void setDataRate250kbps();
    static void setDataRate125kbps();


    /*
     * Is peripheral initialized as I2C master
     * Does not check bit rate or interrupts.
     */
    static bool isInitI2CMaster();
    /*
     * Is I2C master, slave address set, bit rate set and other stuff
     */
    static bool isInitialized();

    static bool isInitToAddressSlave(unsigned int slaveAddress);



    static void enable();
    static void disable();
    static bool isEnabled();


    // Write data to register.
    static void write( const RegisterAddress registerAddress,
                       unsigned const char * const dataOut,
                       const unsigned int count);

    // Read from register into buffer
    static void read( const RegisterAddress registerAddress,
                      unsigned char * const bufferIn,
                      const unsigned int count);

    /*
     * Overloaded, single byte operations.
     */
    static void write( const RegisterAddress registerAddress,
                       const unsigned char value);
    static unsigned char read( const RegisterAddress registerAddress);
};
