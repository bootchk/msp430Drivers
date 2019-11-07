

/*
 * Private methods of I2CDirect
 */


#include "i2cDirect.h"

#include <eusci_b_i2c.h>



bool
I2CDirect::waitForStart() {
    // UCTXIFG0 is set when TXBUF is empty i.e. ready to receive next byte to transmit
    while (!(UCB0IFG & UCTXIFG0)) {

    }
    return true;
}

