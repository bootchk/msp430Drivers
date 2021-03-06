/*
 * ISR for stateMachine implementation of I2C link layer
 */

// Exclude from build unless using stateMachine implementation of I2CTransport
// If you don't exclude, multiply defines symbols for the ISR

// Found is srcExtra directory to keep it from being in directories in other projects that are soft-linked to msp430Drivers/src


#include "stateMachine.h"




#ifdef USE_STATE_MACHINE_ISR


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
            readBufferPtr[bufferIndex++] = rx_val;
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
                  UCB0TXBUF = writeBufferPtr[bufferIndex++];  // side effect is clear TXIFG
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

#endif
