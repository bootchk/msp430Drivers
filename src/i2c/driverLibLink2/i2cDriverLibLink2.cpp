
#include "i2cDriverLibLink2.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "board.h"  // I2CInstanceAddress

#include "../../assert/myAssert.h"

#include "../driverLibEUSCI/EUSCI.h"


#define TRANSACTION_TIMEOUT 200

/*
 * Private, link operations
 */

bool I2CDriverLibLink2::writeOneByte( unsigned char dataByte) {
    bool result;

    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_TRANSMIT_MODE);
    result = EUSCI_B_I2C_masterSendSingleByteWithTimeout(I2CInstanceAddress, dataByte, TRANSACTION_TIMEOUT);
    // !!! assert STOP was sent but may not be complete
    return result;
}

unsigned char I2CDriverLibLink2::readOneByte() {
    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_RECEIVE_MODE);
    // ??? Why not EUSCI_B_I2C_masterReceiveSingleByteWithTimeout
    return EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
    // !!! assert STOP was sent but may not be complete
}

/*
 * Why does DriverLib not have "with timeout" functions for all of this, but some of the functions?
 */
bool I2CDriverLibLink2::readMultipleBytes(unsigned char * buffer, unsigned int count) {
    // !!! Not handling a single byte
    myRequire(count>1);

    // bufferIndex counts up
    unsigned int bufferIndex = 0 ;

    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_RECEIVE_MODE);

    EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);
    // assert START condition sent

    while (bufferIndex != (count - 1) ) {
        /*
         * !!! EUSCI_B_I2C_masterReceiveMultiByteNext just fetches RXBUF, but doesn't care whether RXBUF is full
         * EUSCI_B_I2C_masterReceiveSingle polls for RXIFG, returns RXBUF, which also clears RXIFG
         */
        buffer[bufferIndex] = EUSCI_B_I2C_masterReceiveSingle(I2CInstanceAddress);
        bufferIndex++;
    }
    // assert bufferIndex == count-1
    buffer[bufferIndex] = EUSCI_B_I2C_masterReceiveMultiByteFinish(I2CInstanceAddress);
    // assert STOP was sent
    // !!! assert STOP is complete (see DriverLib code)

    // Hack
    return true;
}



/*
 * At the link layer, just one long transmit, but with first byte distinguished: registerAddress, and following bytes data.
 */
bool I2CDriverLibLink2::writeMultipleBytes(unsigned int registerAddress, unsigned const char * const buffer, unsigned int count) {
    // !!! Must write a byte of data, else should use writeSingleByte()
    myRequire(count>0);

    // bufferIndex counts up
    unsigned int bufferIndex = 0 ;

    bool result;

    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_TRANSMIT_MODE);

    result = EUSCI_B_I2C_masterSendMultiByteStartWithTimeout(I2CInstanceAddress, registerAddress, TRANSACTION_TIMEOUT);
    // assert START condition and registerAddress sent
    if (!result) return result;

    // Continue, sending all data less last byte
    while (bufferIndex != (count - 1) ) {
        /*
         * !!! EUSCI_B_I2C_masterReceiveMultiByteNext just fetches RXBUF, but doesn't care whether RXBUF is full
         * EUSCI_B_I2C_masterReceiveSingle polls for RXIFG, returns RXBUF, which also clears RXIFG
         */
        result = EUSCI_B_I2C_masterSendMultiByteNextWithTimeout(I2CInstanceAddress, buffer[bufferIndex], TRANSACTION_TIMEOUT);
        if (!result) return result;
        bufferIndex++;
    }
    // assert bufferIndex == count-1

    // Send last byte and STOP condition
    result = EUSCI_B_I2C_masterSendMultiByteFinishWithTimeout(I2CInstanceAddress, buffer[bufferIndex], TRANSACTION_TIMEOUT);
    // assert STOP was sent
    // !!! NOT assert STOP is complete (see DriverLib code)
    return result;
}



bool I2CDriverLibLink2::isStopComplete() {
    /*
     * !!! Don't use EUSCI_B_I2C_masterIsStopSent()
     * It returns non-zero (or true) if STOP bit is set, i.e. it is misleading or wrong.
     *
     * !!! Hardcoded EUSCI instance.
     */
    return ( not (UCB0CTLW0 & UCTXSTP));
}





/*
 * Public, transport layer API
 *
 *
 *
 * Each read/write is a separate transaction meaning STOP was sent and completed.
 * Caller does not need to wait for STOP complete.
 */



void I2CDriverLibLink2::read(unsigned int registerAddress, unsigned char * buffer, unsigned int count) {
    /*
     * A read is in sequence:
     * - a I2C write of a register address, to set the hidden "Address Register" in the slave
     * - a I2C read, which reads from the current address set in the slave
     */

    // require EUSCI already init
    // require slave address already set
    // require count > 0

    // Transmit one byte of register address
    writeOneByte(registerAddress);

    /*
     * Now, one byte data and STOP were sent.
     * Wait for STOP to be complete.
     */
    while (not isStopComplete()) ;

    // Read on or more from slave (from register address previously set in slave)

    /*
     * Reading one byte need not be distinguished.
     * But it might be more efficient, since the STOP condition can be sent earlier when we know there is only one byte.
     */
    if (count == 1) {
        *buffer = readOneByte();
    }
    else {
        readMultipleBytes(buffer, count);
    }

    /*
     * STOP was sent but clearing of STP bit may be pending.
     * We wait for it here, so caller does not need to check before the next transaction.
     */
    while (not isStopComplete()) ;
}






void I2CDriverLibLink2::write(unsigned int registerAddress, const unsigned char * const buffer, unsigned int count) {
    /*
     * A write is in sequence:
     * - a I2C write of a register address, to set the hidden "Address Register" in the slave
     * - a I2C write, which writes to the current address set in the slave
     */

    // require EUSCI already init
    // require slave address already set
    // require count > 0

    // Transmit one byte of register address
    writeMultipleBytes(registerAddress, buffer, count);

    /*
     * STOP was sent but clearing of STP bit may be pending.
     * We wait for it here, so caller does not need to check before the next transaction.
     */
    while (not isStopComplete()) ;
}

unsigned char I2CDriverLibLink2::read(unsigned int registerAddress) {
    unsigned char aByte;

    // Transmit one byte of register address
    writeOneByte(registerAddress);
    while (not isStopComplete()) ;
    // Receive one byte of data from that register address
    aByte = readOneByte();
    while (not isStopComplete()) ;
    return aByte;
}


/*
 * Writing one byte of data must be a single transaction (slave address, register address, data)
 * Otherwise the device can't distinguish a single write to set its address register from a single write of data.
 */
void I2CDriverLibLink2::write(unsigned int registerAddress, unsigned const char value) {
    // Pass address of parameter on stack
    write(registerAddress, &value, 1);
    // Since write() waits for stop complete, assert stop is complete
}
