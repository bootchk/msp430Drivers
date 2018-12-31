
#include "i2cTransaction.h"


// #include "../compass/compass.h"

//#include "../pinFunction/spiPins.h"
// #include "../bridge/bridge.h"

/*
 * This is an example using:
 * - Texas Instrument (TI) DriverLib.
 * - TI MSP430 microprocessor family
 * - TI eUSCI_B peripheral (only on some members of micro family)
 * - I2C bus
 *
 * Relevant to any "combined mode" I2C protocol.
 * Relevant to other sensor chips
 */




// DriverLib
#include <eusci_b_i2c.h>
#include <cs.h> // determine clock freq

#include "../src/board.h"   // Module and GPIO defs




static EUSCI_B_I2C_initMasterParam param {
    EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
    8000000,    // freq of clock
    EUSCI_B_I2C_SET_DATA_RATE_100KBPS, // EUSCI_B_I2C_SET_DATA_RATE_400KBPS,
    1, // threshold for auto stop
    EUSCI_B_I2C_NO_AUTO_STOP,
};





// The constant a LIS3MDL chip returns as identifier from register that holds ID
const unsigned int LIS3MDLIdentifier = 0x3D;




/*
 * Read ID register of compass.
 *
 * On i2c bus [START, send compass address, send register address, RESTART, receive register contents, STOP]
 */





/*
 * Configure the master side of the link.
 * To talk to a given slave (parameter: slaveAddress)
 *
 * Ensures:
 * Master device:
 * - is in transmit mode (the default after disabling/enabling)
 * - is enabled
 *
 * Requires LPM5 unlocked before or after calling this, to ensure GPIO config is effective
 */
void configureMasterDevice(unsigned char slaveAddress) {


    // Configuring device requires disabled
    EUSCI_B_I2C_disable(I2CInstanceAddress);

    // Dynamically determine frequency of clock input to I2C module
    // Instead of hardcoding it
    param.i2cClk = CS_getSMCLK();

    EUSCI_B_I2C_initMaster(I2CInstanceAddress, &param);

    /*
     * Configure i2c pins
     *
     * Note that eusci_b must be the primary module for the given pins.
     * For some mcu family members and pins, the eusci_b device could be secondary.
     */
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SDA_PORT, I2C_SDA_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SCL_PORT, I2C_SCL_PIN,
                                                GPIO_PRIMARY_MODULE_FUNCTION);

    /*
     * "Slave address" is a notion of i2c standard.
     * It is 7-bits.
     * It selects the slave.
     * Selecting slave is most relevant on a bus instance having many slaves.
     * Selecting slave is also required even when there is only one slave.
     */

    EUSCI_B_I2C_setSlaveAddress(I2CInstanceAddress, slaveAddress);
    /*
     * A bit, the "R/W bit" is also a notion of i2c standard.
     * It always follows the 7 bits of a slave address, hence sometimes called an eighth bit.
     * It indicates whether master is reading (1) or writing (0) to slave.
     * It accompanies a START bit and slave address,
     * OR a RESTART bit and slave address.
     *
     * Note that:
     * - the master device always generates the clock,
     * - the master device always transmits a slave address on the bus (generating SDA).
     * - the master (not the device, but the higher protocol) may also transmit a subaddress on the bus (generating SDA).
     * It is only the subsequent data bytes that are described by the R/W bit.
     * That is, the R/W bit==1 indicates to the slave that it should begin generating SDA.
     * See LIS3MDL datasheet.
     */

    /*
     * Done configuring.
     * Now enable, so can communicate on the bus.
     * Note that the mode (RW) can and  will be changed by subsequent calls, without disabling and enabling again.
     */
    EUSCI_B_I2C_enable(I2CInstanceAddress);
}

/*
 * Begin using the link so as to subsequently access register(s) in slave device.
 *
 * Sends a preamble forward over the link.
 *
 * Subsequently, you can continue to:
 * - write data forward to the register(s)
 * - read data backwards from the register(s)
 *
 * Parameters:
 * - registerAddress: first register address on slave device
 * - TODO: whether subsequently will be multiple bytes
 * - TODO: whether subsequently is a read or a write to registers
 */
void sendRegisterAccessPreamble(unsigned int registerAddress, bool isMultiByte) {
    /*
     * Mangle the register address (set certain high order bits)
     *
     * i2c expects a byte.
     *
     * A slave interprets 7 LSB bits as register address.
     * (fewer than 7 bits may define address space of the chip.  Depends on chip.  LIS3MDL has a 6 bit address space: [0x00, 0x33] )
     * A slave interprets a bit as autoincrement mode (multiple ops (r/w,  to/from) incremented register addresses)
     * LIS3MDL uses BIT7 (MSB) ==1 to indicate multiple ops i.e. autoincrement
     *
     * bit7==1 indicates multiple ops
     */
    if (isMultiByte) registerAddress |= 128;
    // Else no mangling necessary, this is only one byte transfer at higher protocol level

    /*
     * Send register address, mangled.
     *
     * This:
     * - sends start condition (start bit, slave address, and RW bit==
     * - sends one data byte (the register address)
     * - does NOT send the STOP condition
     * - does require the slave to ACK each other byte
     */
    EUSCI_B_I2C_masterSendMultiByteStart(I2CInstanceAddress, registerAddress);
}




unsigned int receiveOneByte(unsigned int registerAddress) {

    configureMasterDevice(LIS3MDLAddress);

    // Require mode is EUSCI_B_I2C_TRANSMIT_MODE
    // EUSCI_B_I2C_setMode(EUSCI_B_I2C_TRANSMIT_MODE);

    sendRegisterAccessPreamble(registerAddress, false);

    unsigned int result;

#ifdef MULTISTEP
    /*
     * initiate receive
     *
     * This:
     * - switches master to receive mode
     * - sends start condition (start bit, slave address, and RW bit==
     */
    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);

    /*
     * Assert slave will begin generating SDA signal.
     * First slave will generate ACK bit on SDA to acknowledge START.
     * Then slave will generate data byte on SDA.
     * Master device will generate SCL signal and fill RXBUF from SDA signal pulses.
     * After RXBUF if full (one byte), master device will generate SDA with ACK.
     */

    // TODO do we need to wait for data to arrive?
#endif



#ifdef FAILS
    result = EUSCI_B_I2C_masterReceiveMultiByteFinish(I2CInstanceAddress);

#endif
#ifdef FAILS2
    // This works only if you step through it slowly
    /// result = EUSCI_B_I2C_masterReceiveMultiByteNext(I2CInstanceAddress);
    EUSCI_B_I2C_masterReceiveMultiByteStop(I2CInstanceAddress);
#endif
#ifdef FAILS3
    // Blocks until data is ready (RXIFG).  Clears RXIFG
    result = EUSCI_B_I2C_masterReceiveSingle(I2CInstanceAddress);

#endif



#ifdef FAILS4

    // This one sends [RESTART] and polls, and sends stop
    // Fails: returns zero instead of
    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);
    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);
#endif

#define FAILS5
#ifdef FAILS5
    /*
     * Set mode RX
     * Send start condition
     * Wait for start
     * Send stop condition
     * Wait for RXBUF
     * Return RXBUF
     */
    result = EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
#endif

#ifdef TOTRY
    // Next try

    /*
     * Set mode RX
     * Send start
     *
     * !!! Does not wait for start to finish.
     */
    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);
    /*
     * Send stop
     * Wait for stop
     * Wait for RXBUF
     * Return RXBUF
     */
    EUSCI_B_I2C_masterReceiveMultiByteFinish(I2CInstanceAddress);
#endif
    return result;

}


void receiveMultipleBytes(unsigned int registerAddress,
                          unsigned char *buffer,
                          unsigned int length) {
    configureMasterDevice(LIS3MDLAddress);
    sendRegisterAccessPreamble(registerAddress, true);
    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);

    for (unsigned int index = 0; index < length; index++) {
        buffer[index] = EUSCI_B_I2C_masterReceiveMultiByteNext(I2CInstanceAddress);
    }
    EUSCI_B_I2C_masterReceiveMultiByteStop(I2CInstanceAddress);
}


void I2CTransaction::writeByte(unsigned char data) {
    configureMasterDevice(LIS3MDLAddress);

    // assert mode is TX

    // send and stop transaction
    EUSCI_B_I2C_masterSendSingleByte( I2CInstanceAddress, data);

    // Busy wait for completion, since future calls may disrupt transaction that has not completed.
    while (EUSCI_B_I2C_isBusBusy(I2CInstanceAddress)) ;
}

unsigned char I2CTransaction::readByte() {
    unsigned char result;

    result = EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
    return result;

}
