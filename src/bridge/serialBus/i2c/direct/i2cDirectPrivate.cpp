

/*
 * Private methods of I2CDirect
 */


#include "i2cDirect.h"

#include <eusci_b_i2c.h>

#define TIMEOUT 200

static unsigned int timeout;

bool
I2CDirect::waitForStart() {
    timeout = TIMEOUT;

    // Peripheral sets UCTXIFG0 when TXBUF is empty i.e. ready for caller to load next byte to transmit
    // Except if receiver NACKS
    // while bit is not set, decrement timeout, until timeout is zero
    while ((not ( UCB0IFG & UCTXIFG0)) and --timeout )   ;
    return ( timeout != 0 );
}


bool
I2CDirect::waitForReadyToTXNext() {
    return waitForStart();
}

bool
I2CDirect::abortI2C() {
    UCB0CTLW0 |= UCTXSTP;
    return I2CDirect::waitForStopComplete();
}


bool
I2CDirect::waitForStopComplete() {
    timeout = TIMEOUT;
    // Peripheral should clear UCTXSTP after sending STOP condition
    // Except for unknown reasons
    // while bit is set, decrement timeout until timeout is zero
    while ( (UCB0CTLW0 & UCTXSTP) and --timeout)  ;
    return ( timeout != 0 );
}


bool
I2CDirect::waitForReadByteReady() {
    timeout = TIMEOUT;
    // Peripheral should set UCRXIFG after shifting received byte into RXBUF
    // Except for aribtration loss
    // while bit is not set, decrement timeout, until timeout is zero
    while ( (not (UCB0IFG & UCRXIFG)) and --timeout)  ;
    return ( timeout != 0 );
}
