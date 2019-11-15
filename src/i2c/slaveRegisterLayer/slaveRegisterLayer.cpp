
#include "slaveRegisterLayer.h"

#include "../driverLibLinkFromTI/driverLibLinkFromTI.h"

// FUTURE magic buffer size, assert count is less

//namespace slaveRegisterLayer {
// Local buffer to which we have prepended a register address
static unsigned char bufferWithRegAddress[10];
//}


const unsigned int SlaveRegisterLayer::slaveAddress = 0x69;



bool SlaveRegisterLayer::read(unsigned int registerAddress, unsigned char * buffer, unsigned int count){

    // First link operation: send one byte of register address
    // !!! Uses this layers write, with count of zero, i.e. none of given buffer
    if (not SlaveRegisterLayer::write(registerAddress, buffer, 0)) {
        return false;
    }


    // Second link operation: receive bytes from consecutive registers starting at stateful registerAddress

#ifdef ONE_BYTE_LINK_OPERATIONS
    /*
     * Reading one byte need not be distinguished.
     * But it might be more efficient, since the STOP condition can be sent earlier when we know there is only one byte.
     * Depends on the implementations in layer below.
     */
     if (count == 1) {
         *buffer = readOneByte();
     }
     else {
         readMultipleBytes(buffer, count);
     }
#else
     return DriverLibLinkWISR::readMultipleBytes(slaveAddress, buffer, count);
#endif
}



bool SlaveRegisterLayer::write(unsigned int registerAddress, unsigned const char * const buffer, unsigned int count) {
    /*
     * One long transaction.
     *
     * Prepend buffer with register address.
     */
    bufferWithRegAddress[0] = registerAddress;

    // Copy other bytes, shifted by 1
    for (unsigned int i = 0; i < count; i++ ) {
        bufferWithRegAddress[i+1] = buffer[i];
    }

    // Not passing registerAddress as separate parameter, now it is in buffer
    return DriverLibLinkWISR::writeMultipleBytes(slaveAddress, bufferWithRegAddress, count+1);
}
