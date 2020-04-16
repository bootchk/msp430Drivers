

/*
 * Init, configure, and enable aspects of i2cTransport.
 * See also i2cTransportRW.cpp
 */

//******************************************************************************
//   Derived from a MSP430FR243x Demo from
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018


#include "board.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "i2cTransport.h"

#include "../i2cPeripheral.h"
#include "../../pinFunction/i2cPins.h"



#include "../../driverParameters.h"  // I2C bus speed

#include "../../assert/myAssert.h"


#ifdef USE_DRIVERLIB_FOR_LINK
#include "../driverLibLink/i2cDriverLibLink.h"
#elif defined(USE_DRIVERLIB2_FOR_LINK)
#include "../driverLibLink2/i2cDriverLibLink2.h"
#elif defined(USE_DIRECT_FOR_LINK)
#include "../direct/i2cDirect.h"
#elif defined(USE_STATE_MACHINE_FOR_LINK)
#include "../stateMachine.h"
#else
#error "No link implementation defined"
#endif




unsigned int I2CTransport::slaveAddress;


void I2CTransport::enable() { EUSCI_B_I2C_enable(I2CInstanceAddress); }
void I2CTransport::disable() { EUSCI_B_I2C_disable(I2CInstanceAddress); }

// not UCSWRST reset bit is set
bool I2CTransport::isEnabled() { return not (UCB0CTLW0 & UCSWRST); }








void I2CTransport::configurePinsWithExternalPullups() {
    I2CPins::configureWithExternalPullup();
}

void I2CTransport::configurePinsWithInternalPullups() {
    // FUTURE choice of implementation Direct
    I2CPins::configureWithInternalPullup();
}



void I2CTransport::unconfigurePins()
{
    I2CPins::unconfigure();
}



bool I2CTransport::isUnconfigurePins() {
    // FUTURE not hardcoded isUnconfigurePins
    // Only used in assertions?
    return ( // not (P1REN & (BIT2 | BIT3)) and  // No pullup
             ((P1SEL0 & (BIT2 | BIT3)) == 0 ) and// both general purpose (0,0)
             ((P1SEL1 & (BIT2 | BIT3)) == 0 ) and
             ((P1DIR & (BIT2 | BIT3)) == (BIT2 | BIT3)) and  // both out
             ((P1OUT & (BIT2 | BIT3)) == (BIT2 | BIT3))  // both high
             );
}

bool I2CTransport::isConfiguredPinsForModule() {
    return (
             ((P1SEL1 & (BIT2 | BIT3)) == 0 ) // are PSEL1 bits clear
             and ((P1SEL0 & (BIT2 | BIT3)) == (BIT2 | BIT3) )  // are PSEL0 bits set
            );
}







void I2CTransport::initI2CPeripheral(unsigned int slaveAddress)
{
    /*
     * Not require disabled: each init may disable peripheral as part of init()
     */

#ifdef USE_DRIVERLIB_FOR_LINK
    I2CDriverLibLink::initI2CPeripheral(slaveAddress);
    setDataRate125kbps();

#elif defined(USE_DIRECT_FOR_LINK)
    I2CDirect::initI2CPeripheral(slaveAddress);
    // assert data rate is set, and peripheral enabled
#else
    // Other implementations require no init: they init on each transaction
    // Note that other interrupts are enabled by stateMachine.

#endif

    // Not all implementations ensure enabled, caller must do that
}



void I2CTransport::setDataRate250kbps() {

    myRequire( not I2CTransport::isEnabled());

    // require SMCLK selected, at 1Mhz
    // 1Mhz/4 yields 250kbps
    UCB0BRW = 4;
}

void I2CTransport::setDataRate125kbps() {

    myRequire( not I2CTransport::isEnabled());

    // require SMCLK selected, at 1Mhz
    // 1Mhz/8 yields 125kbps
    UCB0BRW = 8;
}







bool I2CTransport::isInitI2CMaster() {
    //                   I2C mode     master   submain clock   always 1
    return (UCB0CTLW0 == UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC);
}

bool I2CTransport::isInitToAddressSlave(unsigned int slaveAddress) {
    return (UCB0I2CSA == slaveAddress);
}

bool I2CTransport::isInitialized(unsigned int slaveAddress) {
#ifdef FULL_INIT_CHECK
    return
            isInitI2CMaster()
            and isInitToAddressSlave(slaveAddress)
            // no autostop    clock low timeout
            and (UCB0CTLW1 == UCCLTO_1 )
            // divisor is four, yielding 250kbps
            and (UCB0BRW == 4)
            ;
#else
    return
            isInitI2CMaster()
            and isInitToAddressSlave(slaveAddress);
#endif
}






