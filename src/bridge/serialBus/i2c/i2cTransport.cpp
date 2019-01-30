



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

#include "../../../assert/myAssert.h"


void I2CMaster::enable() { EUSCI_B_I2C_enable(I2CInstanceAddress); }
void I2CMaster::disable() { EUSCI_B_I2C_disable(I2CInstanceAddress); }

// not UCSWRST reset bit is set
bool I2CMaster::isEnabled() { return not (UCB0CTLW0 & UCSWRST); }




void I2CMaster::read(
                        const RegisterAddress registerAddress,
                        unsigned char * const buffer,
                        unsigned int count)
{
    // require init() was called

    I2CStateMachine::init(registerAddress, buffer, count, false);  // false means a read
    I2CStateMachine::initialTransition();

    // assert buffer is filled by device
}


void I2CMaster::write(
        const RegisterAddress registerAddress,
        unsigned char * const buffer, // buffer data is const but stateMachine wants a buffer that is changeable
        const unsigned int count)
{

    I2CStateMachine::init(registerAddress, buffer, count, true);    // true means write
    I2CStateMachine::initialTransition();
}






void I2CMaster::configurePins()
{
#define USE_DRIVER_LIB
#ifdef USE_DRIVER_LIB
    I2CPins::configure();

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

void I2CMaster::unconfigurePins()
{
    I2CPins::unconfigure();
}

/*
 * SMCLK defaults to DCO freq of 1Mhz
 *
 * FUTURE fast data rate
 * FUTURE persist in FRAM
 */
EUSCI_B_I2C_initMasterParam params = {
                                      EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
                                      1000000,    // supplied clock freq
                                      EUSCI_B_I2C_SET_DATA_RATE_100KBPS,    // desired data rate
                                      EUSCI_B_I2C_NO_AUTO_STOP
};

void I2CMaster::initI2CPeripheral(unsigned char slaveAddress)
{
    myRequire(not isEnabled());

#define   USE_DRIVER_LIB
#ifdef USE_DRIVER_LIB
    EUSCI_B_I2C_initMaster(I2CInstanceAddress,
                           &params);

    EUSCI_B_I2C_setSlaveAddress(I2CInstanceAddress, slaveAddress);

    // Enable NACK interrupt.
    // So we can catch bus errors.
    EUSCI_B_I2C_enableInterrupt(I2CInstanceAddress, EUSCI_B_I2C_NAK_INTERRUPT);

    /*
     * Note that other interrupts are enabled by stateMachine.
     */

    // Not ensure enabled, caller must do that

#else
    // Works.  Code from TI.
    UCB0CTLW0 |= UCSWRST;  // disable
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    UCB0BRW = 160;                            // fSCL = SMCLK/160 = ~100kHz
    UCB0I2CSA = slaveAddress;                   // Slave Address
    UCB0IE |= UCNACKIE;     // lkk why enable NACK interrupt?
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
#endif

}


