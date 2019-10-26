
#include "i2cDriverLibLink.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "board.h"  // I2CInstanceAddress

#include "../../../assert/myAssert.h"


void I2CDriverLibLink::writeOneByte( unsigned char dataByte) {
    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_TRANSMIT_MODE);
    EUSCI_B_I2C_masterSendSingleByte(I2CInstanceAddress, dataByte);
}

unsigned char I2CDriverLibLink::readOneByte() {
    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_RECEIVE_MODE);
    return EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
    // !!! assert STOP may not be complete
}

/*
 * Why does DriverLib not have "with timeout" functions for all of this, but some of the functions?
 */
void I2CDriverLibLink::readMultipleBytes(unsigned char * buffer, unsigned int count) {
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
}



bool I2CDriverLibLink::isStopComplete() {
    /*
     * !!! Don't use EUSCI_B_I2C_masterIsStopSent()
     * It returns non-zero (or true) if STOP bit is set, i.e. it is misleading or wrong.
     *
     * !!! Hardcoded EUSCI instance.
     */
    return ( not (UCB0CTLW0 & UCTXSTP));
}


void I2CDriverLibLink::read(unsigned int registerAddress, unsigned char * buffer, unsigned int count) {
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
