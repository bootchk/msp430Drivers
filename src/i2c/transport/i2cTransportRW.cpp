/*
 * The read/write part of i2cTransport implementation
 */


#include "i2cTransport.h"

#include "../../assert/myAssert.h"





#ifdef USE_DRIVERLIB_FOR_LINK
#include "../driverLibLink/i2cDriverLibLink.h"
#include "../i2cPeripheral.h"
#elif defined(USE_DRIVERLIB2_FOR_LINK)
#include "../driverLibLink2/i2cDriverLibLink2.h"
#include "../slaveRegisterLayer/slaveRegisterLayer.h"
#elif defined(USE_DIRECT_FOR_LINK)
#include "../direct/i2cDirect.h"
#elif defined(USE_STATE_MACHINE_FOR_LINK)
#include "../stateMachine.h"
#else
#error "No link implementation defined"
#endif





bool I2CTransport::read(
                        const RegisterAddress registerAddress,
                        unsigned char * const buffer,
                        unsigned int count)
{
    // TODO some implentations init and enable peripheral per call
    //myRequire( isInitialized() );
    //myRequire( isEnabled() );

    // I found that STPIFG and BIT9IFG are set.  This assertion is not valid
    // myRequire( I2CPeripheral::isNoInterruptFlag() );

    // TODO some implementation do not ensure this???  I2CPeripheral::waitUntilPriorTransportComplete();

    // This apparently is flawed.
    //I2CPeripheral::waitUntilBusReady();

    // TODO some implementation do not ensure this???  I2CPeripheral::clearInterruptFlags();

#ifdef USE_DRIVERLIB_FOR_LINK
    I2CDriverLibLink::read(registerAddress, buffer, count);
#elif defined(USE_DRIVERLIB2_FOR_LINK)
    SlaveRegisterLayer::read(registerAddress, buffer, count);
    // TODO return result if nacked
    return true;
#elif defined(USE_DIRECT_FOR_LINK)
    return I2CDirect::readFromAddress(registerAddress, buffer, count);
#elif defined(USE_STATE_MACHINE_FOR_LINK)
    I2CStateMachine::initForRead(registerAddress, buffer, count);
    I2CStateMachine::initialTransition();
#endif

    // assert buffer is filled by device
}


bool I2CTransport::write(
        const RegisterAddress registerAddress,
        unsigned const char * const buffer, // buffer data is const but stateMachine wants a buffer that is changeable
        const unsigned int count)
{
    // TODO init per call
    //myRequire( isInitialized() );
    //myRequire( isEnabled() );

    // TODO some implementation do not ensure this???  I2CPeripheral::waitUntilPriorTransportComplete();

#ifdef USE_DRIVERLIB_FOR_LINK
    I2CDriverLibLink::write(registerAddress, buffer, count);
#elif defined(USE_DRIVERLIB2_FOR_LINK)
    SlaveRegisterLayer::write(registerAddress, buffer, count);
    // TODO return result if nacked
        return true;
#elif defined(USE_DIRECT_FOR_LINK)
    return I2CDirect::writeToAddress(registerAddress, buffer, count);
#elif defined(USE_STATE_MACHINE_FOR_LINK)
    I2CStateMachine::initForWrite(registerAddress, buffer, count);
    I2CStateMachine::initialTransition();
#endif
}




/*
 * Single byte transport
 */
unsigned char I2CTransport::read(const RegisterAddress registerAddress)
{
#ifdef USE_DRIVERLIB_FOR_LINK
    I2CPeripheral::waitUntilPriorTransportComplete();
    return I2CDriverLibLink::read(registerAddress);
#elif defined(USE_DRIVERLIB2_FOR_LINK)
    return I2CDriverLibLink2::read(registerAddress);
#elif defined(USE_DIRECT_FOR_LINK)
    return I2CDirect::readFromAddress(registerAddress);
#elif defined(USE_STATE_MACHINE_FOR_LINK)
    unsigned char buffer[1];
    I2CStateMachine::initForRead(registerAddress, buffer, 1);
    I2CStateMachine::initialTransition();
    return buffer[0];
#endif
}

void I2CTransport::write(const RegisterAddress registerAddress, const unsigned char value)
{
#ifdef USE_DRIVERLIB_FOR_LINK
    I2CPeripheral::waitUntilPriorTransportComplete();
    I2CDriverLibLink::write(registerAddress, value);
#elif defined(USE_DRIVERLIB2_FOR_LINK)
    I2CDriverLibLink2::write(registerAddress, value);
#elif defined(USE_DIRECT_FOR_LINK)
    I2CDirect::writeToAddress(registerAddress, value);
#elif defined(USE_STATE_MACHINE_FOR_LINK)
    I2CStateMachine::initForWrite(registerAddress, &value, 1);
    I2CStateMachine::initialTransition();
#endif
}
