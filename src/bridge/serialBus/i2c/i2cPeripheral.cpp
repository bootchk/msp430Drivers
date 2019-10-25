
#include "i2cPeripheral.h"

#include "board.h" // I2CInstanceAddress

// Driverlib
#include <eusci_b_i2c.h>



void I2CPeripheral::clearInterruptFlags() {
    // write interrupt generator clears all flags
    // !!! Although datasheet also says it is type R meaning read only?
    UCB0IV = 0;
}

bool I2CPeripheral::isNoInterruptFlag() {
    return UCB0IFG == 0;
}


unsigned int  I2CPeripheral::getInterruptFlags() {
    return UCB0IFG;
}

bool I2CPeripheral::isBusBusy() {
    return (EUSCI_B_I2C_isBusBusy(I2CInstanceAddress) == EUSCI_B_I2C_BUS_BUSY);
    // return (UCB0STATW & UCBBUSY);
}

void I2CPeripheral::waitUntilBusReady() {
    // TODO should send STP while waiting
    while ( I2CPeripheral::isBusBusy()) {
        // bus is busy (start was seen), but we should be the only master.
       // send stop to tell other masters and reset the busy flag.
       UCB0CTLW0 |= UCTXSTP;
    }
    // If we sent stop, then stop flag is set.  Clear it.
    clearInterruptFlags();
}



void I2CPeripheral::waitUntilPriorTransportComplete() {
    /*
     * The prior transaction set the STOP bit, and the peripheral will clear the bit after signaling STOP on wire.
     */
    while (UCB0CTLW0 & UCTXSTP)  ;
}

