
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
#include "i2cTransport.h"

#include "../../../assert/myAssert.h"


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
uint8_t * bufferPtr;

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




void I2CStateMachine::init(const uint8_t registerAddress,
               uint8_t * const buffer,
               uint8_t count,
               bool isTransactionSend ) {

    // Require driver enabled
    myAssert(I2CTransport::isEnabled());

    state = SendingStart;
    command = registerAddress;
    byteCounter = count;
    transactionIsSend = isTransactionSend;
    bufferPtr = buffer;
    bufferIndex = 0;
}









#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

  /*
   * Read the interrupt vector generator (IVG.)
   * If more than one interrupt, only the highest priority will be read.
   * Reading clears the corresponding IFG bit.
   * Note that other operations (such as reading RXBUF) also clear corresponding IFG bit.
   */
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts

    case USCI_I2C_UCALIFG:       // Vector 2: arbitration lost
    case USCI_I2C_UCNACKIFG:     // Vector 4: NACK
        // TODO handle gracefully:  return error, and reset driver
        myAssert(false);
        break;

    case USCI_I2C_UCCLTOIFG:
        // Clock low too long (slave stretching clock incorrectly?)
        myAssert(false);
        break;


    case USCI_I2C_UCSTTIFG:
    case USCI_I2C_UCSTPIFG:
        // Start and stop received.  Since we are master, not expected
        // Also, USCTPIFG would be set if we were using autostop generation, which we are not
        myAssert(false);
        break;

    case USCI_I2C_UCRXIFG3:
    case USCI_I2C_UCTXIFG3:
    case USCI_I2C_UCRXIFG2:
    case USCI_I2C_UCTXIFG2:
    case USCI_I2C_UCRXIFG1:
    case USCI_I2C_UCTXIFG1:
        // Unexpected
        myAssert(false);
        break;


    // receiving.  Flag set and interrupt when device has received a byte into device buffer.
    case USCI_I2C_UCRXIFG0:
        myAssert(state == ReceivingData);

        uint8_t rx_val;
        // Read device buffer UCB0RXBUF, side effect is clear RXIFG
        rx_val = UCB0RXBUF;

        // A byte was received and byteCounter should be positive.
        // In case we ever get this interrupt in the wrong state,
        // checking byteCounter positive prevents us from overflowing buffer.
        if (byteCounter)
        {
            // Copy from device buffer to app buffer
            bufferPtr[bufferIndex++] = rx_val;
            byteCounter--;
        }

        if (byteCounter == 1)
        {
            // Last byte yet to be received.
            // Tell device to send stop at end of next received byte.
            // If there was only one byte to receive, we already set UTXSTP (see "Sending Restart")
            UCB0CTLW0 |= UCTXSTP;  // lhs is register, rhs is bit value
            // RXIE still set, next action is another RXIFG interrupt
        }
        else if (byteCounter == 0)
        {
            // Stop condition set earlier
            UCB0IE &= ~UCRXIE;    // disable further RX interrupt

            state = Idle;

            __bic_SR_register_on_exit(LPM0_bits);      // Exit LPM0.  Was CPUOFF
        }
        break;

    /*
     * TXIFG means ready for more TX data
     * - START is being sent
     * - OR previous TX data has vacated TXBUF
     */
    case USCI_I2C_UCTXIFG0:
        switch (state)
        {
          case SendingStart:
              // TXIFG means START bit and slave address being sent

              // Queue up command (i.e. register address) to send
              // Side effect is clear TXIFG
              // Next action is TXIFG set and interrupt when command has been sent
              UCB0TXBUF = command;

              if (not transactionIsSend)
                  state = SendingRestart;   // Need to start receiving now
              else
                  state = TransmittingData; // Continue to transmision with the data in Transmit Buffer
              break;
              // exit ISR without leaving LPM

          case SendingRestart:
              // Slave address and register address were sent, send restart and then receive data from slave
              UCB0IE |= UCRXIE;              // Enable RX interrupt
              UCB0IE &= ~UCTXIE;             // Disable TX interrupt
              UCB0CTLW0 &= ~UCTR;            // Switch to receiver
              state = ReceivingData;          // State state is to receive data
              UCB0CTLW0 |= UCTXSTT;          // Send repeated start
              if (byteCounter == 1)
              {
                  // To avoid delay for bus stall, must send stop since this is the N-1 byte.
                  // This means send stop at conclusion of next byte received.
                  while((UCB0CTLW0 & UCTXSTT));
                  UCB0CTLW0 |= UCTXSTP;      // Send stop condition
              }
              // Next action will be RXIFG flag and interrupt
              break;

          case TransmittingData:
              if (byteCounter)
              {
                  UCB0TXBUF = bufferPtr[bufferIndex++];  // side effect is clear TXIFG
                  byteCounter--;
              }
              else
              {
                  //Done with packet and therefore done with transport

                  UCB0CTLW0 |= UCTXSTP;     // Send stop condition.
                  // Next transaction must not start until TXSTP is clear!!!  See waitUntilPriorTransportComplete()

                  state = Idle;
                  UCB0IE &= ~UCTXIE;                       // disable TX interrupt
                  __bic_SR_register_on_exit(LPM0_bits);      // Exit LPM0
              }
              break;

          default:
              myAssert(false);    // State machine error
              __no_operation();
              break;
        }
        break;


    default:
        myAssert(false);    // Unhandled interrupt
        __no_operation();
        break;
  }
}
