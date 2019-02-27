
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




void I2CStateMachine::waitUntilPriorTransportComplete() {
    /*
     * The prior transaction set the STOP bit, and the peripheral will clear the bit after issuing STOP on wire.
     */
    while (UCB0CTLW0 & UCTXSTP)  ;
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


  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts

    case USCI_I2C_UCALIFG:       // Vector 2: arbitration lost
    case USCI_I2C_UCNACKIFG:     // Vector 4: NACK
        // TODO handle gracefully:  return error, and reset driver
        myAssert(false);
      break;

    case USCI_I2C_UCSTTIFG:
    case USCI_I2C_UCSTPIFG:
        // Start and stop received.  Since we are master, not expected
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

    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        uint8_t rx_val;
        // Read UCB0RXBUF to clear RXIFG
        rx_val = UCB0RXBUF;
        if (byteCounter)
        {
          bufferPtr[bufferIndex++] = rx_val;
          byteCounter--;
        }

        if (byteCounter == 1)
        {
          UCB0CTLW0 |= UCTXSTP;
        }
        else if (byteCounter == 0)
        {
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
              // Next TXIFG when command has been sent
              UCB0TXBUF = command;

              if (not transactionIsSend)
                  state = SendingRestart;   // Need to start receiving now
              else
                  state = TransmittingData; // Continue to transmision with the data in Transmit Buffer
              break;

          case SendingRestart:
              UCB0IE |= UCRXIE;              // Enable RX interrupt
              UCB0IE &= ~UCTXIE;             // Disable TX interrupt
              UCB0CTLW0 &= ~UCTR;            // Switch to receiver
              state = ReceivingData;          // State state is to receive data
              UCB0CTLW0 |= UCTXSTT;          // Send repeated start
              if (byteCounter == 1)
              {
                  //Must send stop since this is the N-1 byte
                  while((UCB0CTLW0 & UCTXSTT));
                  UCB0CTLW0 |= UCTXSTP;      // Send stop condition
              }
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
                  UCB0CTLW0 |= UCTXSTP;     // Send stop condition
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
