
#include "i2cDriverLibLink.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "board.h"  // I2CInstanceAddress


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
    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_TRANSMIT_MODE);
    EUSCI_B_I2C_masterSendSingleByte(I2CInstanceAddress, registerAddress);

    /*
     * Now, one byte data and STOP were sent.
     * But wait for STOP to be complete.
     */
    // TODO

    // Read multiples bytes from slave (from register address previously set in slave)
    EUSCI_B_I2C_setMode(I2CInstanceAddress, EUSCI_B_I2C_RECEIVE_MODE);
    if (count == 1) {
        *buffer = EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
    }
    else {
        EUSCI_B_I2C_masterReceiveStart(I2CInstanceAddress);
        // TODO
    }

    /*
     * STOP was sent but clearing of STP bit may be pending.
     */

}
