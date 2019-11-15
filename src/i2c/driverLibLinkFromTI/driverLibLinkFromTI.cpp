/*
 * Derived from TI  MSP430FR4133 Demo - USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
 *
 * Features:
 * - master only
 * - one slave, slave address hardcoded
 * - uses auto stop
 * - uses ISR
 */

#include "driverLibLinkFromTI.h"

// Depends on MCU clocks
// #include "../../clock/submainClock.h"

#include "../../assert/myAssert.h"

#include "driverlib.h"  // CS, GPIO, and eUSCI

#include "board.h"  // configuration of pullups
#include "../../pinFunction/i2cPins.h"


// FUTURE bit rate not hard coded
#define BIT_RATE EUSCI_B_I2C_SET_DATA_RATE_100KBPS


/*
 * This configures pin each session.
 * Also note that TI recommends configuring pins after initializing peripheral and before enabling peripheral,
 * but this does not.
 */
/*
 * FUTURE
 * Migrate config pins to once per session.
 * Migrate initialization of peripheral to once per session instead of once per R/W transaction.
 */


namespace {
// local state set by init(), accessed by ISR
unsigned char* localBufferPtr;
unsigned int localLength;
unsigned int localBufferIndex;  // increment up, after using (postincrement)
bool isSend;    // mode: sending or receiving
bool wasError;  // result, set by ISR
unsigned int countRetries;   // Counter for retries after nacks

// state init once per run, set by ISR
unsigned int countNacks = 0;
}


void DriverLibLinkWISR::initI2CPins() {
#ifdef I2C_HAS_EXTERNAL_PULLUPS
    I2CPins::configureWithExternalPullup();
#elif defined(I2C_HAS_INTERNAL_PULLUPS)
    I2CPins::configureWithInternalPullup();
#else
#error "No config of pullups in board.h"
#endif
}


#ifdef ORIGINAL_TI_EXAMPLE
    /*
     * Set P1.2, 1.33 to input with function, (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1,
            GPIO_PIN2 + GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION
        );
    // ??? TI demo for masterTXMultiple does it a little differently
#endif



void DriverLibLinkWISR::setSlaveAddress(unsigned int slaveAddress) {
    // Slave address converted to 8 bits, only 7-bits valid for most I2C busses
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, slaveAddress );
}

void DriverLibLinkWISR::waitForPriorTransactionComplete() {
    while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE));
}


void
DriverLibLinkWISR::initForRead (
        unsigned int slaveAddress,
        unsigned char* buffer,
        unsigned int count)
{
    // local state
    localLength = count;
    localBufferPtr = buffer;
    localBufferIndex = 0;
    isSend = false;
    wasError = false;
    countRetries = 0;

    initI2CPins();

    // assert is PMM_unlockLPM5();

    // Require SMCLK is running.  Reset condition is: running at 1Mhz, divider 1
    // SubmainClock::init();
    // SubmainClock::turnOn();

    EUSCI_B_I2C_initMasterParam param = {0};

    /*
     * The driverlib function calculates the setting of the eUSCI divider from actual MCU clock freq and desired bit rate.
     * Less robustly, you can pass constants that should reflect the actual MCU clock freq.
     */
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    uint32_t smclkFreq = CS_getSMCLK();
    // typically 1Mhz
    myAssert(smclkFreq > 0);
    param.i2cClk = smclkFreq;
    // param.i2cClk = 10000000;
    param.dataRate = BIT_RATE;
    param.byteCounterThreshold = count;
    param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    setSlaveAddress(slaveAddress);

    //Set Master in receive mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
        EUSCI_B_I2C_RECEIVE_MODE
        );

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
        EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
        EUSCI_B_I2C_NAK_INTERRUPT
        );

    //Enable master Receive interrupt
    EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE,
        EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
        EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
        EUSCI_B_I2C_NAK_INTERRUPT
        );
}


// Not using autostop
void DriverLibLinkWISR::initForWrite (
        unsigned int slaveAddress,
        unsigned char* buffer,
        unsigned int count)
{
    // local state accessed by ISR
    localLength = count;
    localBufferPtr = buffer;
    localBufferIndex = 0;
    isSend = true;
    wasError = false;
    countRetries = 0;

    initI2CPins();

    // assert is PMM_unlockLPM5();

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = BIT_RATE;
    // For writing, not using autostop.  Why not???
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    //param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    setSlaveAddress(slaveAddress);

    //Set Master in transmit mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
                        EUSCI_B_I2C_TRANSMIT_MODE
        );

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE,
                EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                EUSCI_B_I2C_NAK_INTERRUPT
                );
    //Enable master transmit interrupt
    EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE,
                EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                EUSCI_B_I2C_NAK_INTERRUPT
                );
}




bool DriverLibLinkWISR::readMultipleBytes(
        unsigned int slaveAddress,
        unsigned char * buffer,
        unsigned int count) {

  initForRead(slaveAddress, buffer, count);

  waitForPriorTransactionComplete();

  EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);

  // Enter LPM0 w/ interrupt
  __bis_SR_register(CPUOFF+GIE);

  // ISR will exit LPM0 after receiving last byte or on error
  return (not wasError);
}



bool DriverLibLinkWISR::writeMultipleBytes(
        unsigned int slaveAddress,
        unsigned char * buffer,
        unsigned int count) {

    initForWrite(slaveAddress, buffer, count);

    waitForPriorTransactionComplete();

    // Note passing first byte to TX
    EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, buffer[localBufferIndex++]);

    // Enter LPM0 w/ interrupt
    __bis_SR_register(CPUOFF+GIE);

    // ISR will exit LPM0 after RX or TX last byte or on error
    return (not wasError);
}








#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCIB0_ISR(void)
{
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
	{
        case USCI_NONE:             // No interrupts break;
            break;
        case USCI_I2C_UCALIFG:      // Arbitration lost
            // FUTURE abort
            break;
        case USCI_I2C_UCNACKIFG:    // NAK received (master only)
            countNacks++;
            countRetries++;
            if (countRetries > 5) {
                wasError = true;
                __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
            }
            else {
                // Retry
                // TODO is this restarting?  attempts until success?
                if (isSend) {
                    // The original from TI example, but ???? what about the data already sent?
                    // EUSCI_B_I2C_masterSendStart(EUSCI_B0_BASE);

                    // Send previous byte again.  localBufferIndex is already incremented.
                     EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, localBufferPtr[localBufferIndex - 1]);
                }
                else {
                    EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);
                }
                // Return to LPM0
            }
            break;

        case USCI_I2C_UCSTTIFG:     // START condition detected with own address (slave mode only)
            break;
        case USCI_I2C_UCSTPIFG:     // STOP condition detected (master & slave mode)
            break;

        case USCI_I2C_UCRXIFG3:     // RXIFG3
        case USCI_I2C_UCTXIFG3:     // TXIFG3
        case USCI_I2C_UCRXIFG2:     // RXIFG2
        case USCI_I2C_UCTXIFG2:     // TXIFG2
        case USCI_I2C_UCRXIFG1:     // RXIFG1
        case USCI_I2C_UCTXIFG1:     // TXIFG1
            // Unexpected but harmless?
            break;

        // Receive
        case USCI_I2C_UCRXIFG0:     // RXIFG0
            // Copy received byte from peripheral to memory
            localBufferPtr[localBufferIndex++] = EUSCI_B_I2C_masterReceiveSingle(EUSCI_B0_BASE);
		    if (localBufferIndex >= localLength) {
		        // For safety (in case we get extra interrupts, reset index
			    localBufferIndex = 0;
			    __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
		    }
            break; // Vector 24: RXIFG0 break;



        // Send
        case USCI_I2C_UCTXIFG0:     // TXIFG0
            // Previous byte shifted out of peripheral register (wire transfer may not be done)

            if (localBufferIndex >= localLength) {
                // For safety (in case we get extra interrupts, reset index
                localBufferIndex = 0;
                // Engender STOP condition after wire transfer of last byte
                EUSCI_B_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
                // Exit LPM0
                __bic_SR_register_on_exit(CPUOFF);
            }
            else {
                // More bytes to send
                // Copy next byte from memory to peripheral register
                EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, localBufferPtr[localBufferIndex++]);
                // stay in LPM0 until TXIFG again or NACKIFG
            }
            break;


        case USCI_I2C_UCBCNTIFG:    // Byte count limit reached (UCBxTBCNT)
            // Should be concurrent with RXIFG for last byte received?
            break;
        case USCI_I2C_UCCLTOIFG:    // Clock low timeout - clock held low too long
            break;
        case USCI_I2C_UCBIT9IFG:    // Generated on 9th bit of a transmit (for debugging)
            break;
        default:
            break;
	}
}

