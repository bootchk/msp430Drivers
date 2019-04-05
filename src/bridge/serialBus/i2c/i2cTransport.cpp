



//******************************************************************************
//   Derived from a MSP430FR243x Demo from
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018


#include "board.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "i2cTransport.h"
#include "../../../pinFunction/i2cPins.h"

// Implementation is by a stateMachine (which implements link layer)
#include "stateMachine.h"

#include "../../../driverParameters.h"  // I2C bus speed

#include "../../../assert/myAssert.h"



#define USE_DRIVER_LIB




namespace {
bool _isInitialized = false;
}



void I2CTransport::enable() { EUSCI_B_I2C_enable(I2CInstanceAddress); }
void I2CTransport::disable() { EUSCI_B_I2C_disable(I2CInstanceAddress); }

// not UCSWRST reset bit is set
bool I2CTransport::isEnabled() { return not (UCB0CTLW0 & UCSWRST); }






void I2CTransport::read(
                        const RegisterAddress registerAddress,
                        unsigned char * const buffer,
                        unsigned int count)
{
    myRequire( isInitialized() );

    I2CStateMachine::waitUntilPriorTransportComplete();
    I2CStateMachine::init(registerAddress, buffer, count, false);  // false means a read
    I2CStateMachine::initialTransition();

    // assert buffer is filled by device
}


void I2CTransport::write(
        const RegisterAddress registerAddress,
        unsigned char * const buffer, // buffer data is const but stateMachine wants a buffer that is changeable
        const unsigned int count)
{
    myRequire( isInitialized() );

    I2CStateMachine::waitUntilPriorTransportComplete();
    I2CStateMachine::init(registerAddress, buffer, count, true);    // true means write
    I2CStateMachine::initialTransition();
}






void I2CTransport::configurePins()
{

#ifdef USE_DRIVER_LIB

#ifdef I2C_HAS_EXTERNAL_PULLUPS
    I2CPins::configureWithExternalPullup();
#else
    I2CPins::configureWithInternalPullup();
#endif

#else
    // Works.  Code from TI.

    // I2C pins
    // Port 1 pins 2 and 3 the primary module function
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);

    // Pullup internal (30k ohm)
    // No external pullups
    // I2C is open-collector, requires pull
    // REN, OUT, == 1 and DIR ==0 means input, pull enabled, pull is up
    P1OUT |= BIT2 | BIT3;
    P1REN |= BIT2 | BIT3;
    P1DIR &= ~(BIT2 | BIT3);    // bit clear
#endif
}

void I2CTransport::unconfigurePins()
{
    I2CPins::unconfigure();
    _isInitialized = false;
}

bool I2CTransport::isUnconfigurePins() {
    // TODO hardcoded
    return ( // not (P1REN & (BIT2 | BIT3)) and  // No pullup
             ((P1SEL0 & (BIT2 | BIT3)) == 0 ) and// both general purpose (0,0)
             ((P1SEL1 & (BIT2 | BIT3)) == 0 ) and
             ((P1DIR & (BIT2 | BIT3)) == (BIT2 | BIT3)) and  // both out
             ((P1OUT & (BIT2 | BIT3)) == (BIT2 | BIT3))  // both high
             );
}
















/*
 * SMCLK defaults to DCO freq of 1Mhz
 *
 * FUTURE fast data rate
 *
 * I found:
 * bus length of 3 inches, and internal pullup of 30kohms, max data rate is less than 100kbps (say 10kbps)
 * bus length of 1 inches, and internal pullup of 30kohms, max data rate is less than 100kbps (say 50kbps)
 * Probably should just use external 10kohm pullups.
 */
/*
 * This is constant, but the function to which it is passed does not allow const.
 * So we declare persistent so it goes in FRAM.
 */
#pragma PERSISTENT
EUSCI_B_I2C_initMasterParam params = {
                                      EUSCI_B_I2C_CLOCKSOURCE_SMCLK,        // clock is: submain
                                      1000000,                              // supplied clock freq
                                      DriverConstant::I2CBusSpeed,          // desired data rate
                                      0,                                    // autostop threshold
                                      EUSCI_B_I2C_NO_AUTO_STOP
};

/*
 * A DriverLib example sets clock freq :  param.i2cClk = CS_getSMCLK();
 * I assume that is also the desired frequency, not the actual measured frequency.
 */



#define USE_DRIVER_LIB
void I2CTransport::initI2CPeripheral()
{
    myRequire(not isEnabled());


#ifdef USE_DRIVER_LIB
    EUSCI_B_I2C_initMaster(I2CInstanceAddress,  &params);

    // slave address from board.h
    EUSCI_B_I2C_setSlaveAddress(I2CInstanceAddress, RTCBusAddress);

    // Enable NACK interrupt.  So we can catch bus errors.
    EUSCI_B_I2C_enableInterrupt(I2CInstanceAddress, EUSCI_B_I2C_NAK_INTERRUPT);

    // So we can catch slave gone wild.
    // Enable "clock low" interrupt.
    EUSCI_B_I2C_enableInterrupt(I2CInstanceAddress, EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT );
    // Set timeout
    EUSCI_B_I2C_setTimeout(I2CInstanceAddress, EUSCI_B_I2C_TIMEOUT_28_MS);

    /*
     * Note that other interrupts are enabled by stateMachine.
     */

    // Not ensure enabled, caller must do that

#else
    // Works.  Code from TI.
    UCB0CTLW0 |= UCSWRST;  // disable
    /*
     * UCMODE_3 => I2C
     * UCMST => master
     * UCSYNC => synchronous
     */
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    UCB0BRW = 160;                            // fSCL = SMCLK/160 = ~100kHz
    UCB0I2CSA = slaveAddress;                   // Slave Address
    UCB0IE |= UCNACKIE;     // lkk why enable NACK interrupt?
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
#endif

    _isInitialized = true;
}


bool I2CTransport::isInitialized() { return _isInitialized; }
