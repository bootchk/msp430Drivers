
/*
 * State machine for I2C Transport
 * Using eUSCI_B peripheral on MSP430
 *
 * State machine uses interrupts and ISR
 * Most transitions are triggered by interrupts.
 *
 * !!! the eUSCI is double buffered.
 * When events (interrupt flags set), it is time to queue up a next byte
 * ??? and you must read a received byte after the STOP event
 * TODO revise these comments.
 */

#include <msp430.h>
#include <inttypes.h>

#include "stateMachine.h"


#include "../assert/myAssert.h"


/*
 * States of state machine.
 *
 * Pared to a bare minimum.
 */

typedef enum I2C_ModeEnum{
    Idle,
    SendingStart,
    TransmittingData,
    SendingRestart,
    ReceivingData,
} I2CTransportState;






// Local variables

// Register Address/Command to transmit as first byte of each transport
uint8_t command;

/*
 * Buffer owned by caller.
 * Driver knows pointer.
 * Same buffer used for send and receive transactions (I2C is half duplex.)
 * You must use the contents before starting another transaction.
 */
uint8_t * readBufferPtr;
const uint8_t* writeBufferPtr;

uint8_t byteCounter;    // count of bytes remaining to transact
uint8_t bufferIndex;

bool transactionIsSend;

I2CTransportState state;

// TODO combine bufferIndex and byteCounter into one variable





void I2CStateMachine::initialTransition() {
    // require UCB0I2CSA set to slave address, i.e. require eUSCI_B peripheral initialized

    myRequire(state==SendingStart);

    // Interrupts */
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);      // Enter LPM0 w/ interrupts

    // Subsequent transitions are taken in the ISR
    // The ISR does not exit LPM0 until the I2C transaction is complete (or fails??)

    // The next action will be UCTXIFG flag and interrupt when ready for TXBUF to be filled with a byte of register address on slave.
}




void I2CStateMachine::initForRead(const uint8_t registerAddress,
               uint8_t * const buffer,
               uint8_t count
               ) {

    // This just sets initial state, and has no requires

    state = SendingStart;
    command = registerAddress;
    byteCounter = count;
    transactionIsSend = false;
    readBufferPtr = buffer; // <<<<<
    bufferIndex = 0;
}

void I2CStateMachine::initForWrite(const uint8_t registerAddress,
               const uint8_t * const buffer,
               uint8_t count
               ) {

    // This just sets initial state, and has no requires

    state = SendingStart;
    command = registerAddress;
    byteCounter = count;
    transactionIsSend = true;
    writeBufferPtr = buffer; // <<<<<
    bufferIndex = 0;
}





