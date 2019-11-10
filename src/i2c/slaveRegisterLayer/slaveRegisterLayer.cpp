
#include "slaveRegisterLayer.h"

#include "../driverLibLinkFromTI/driverLibLinkFromTI.h"

// TODO magic buffer size, not checked
static unsigned char bufferWithRegAddress[10];




void SlaveRegisterLayer::read(unsigned int registerAddress, unsigned char * buffer, unsigned int count){

    // First link operation: send one byte of register address
    // !!! Uses this layers write, with count of zero, i.e. none of given buffer
    SlaveRegisterLayer::write(registerAddress, buffer, 0);


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
     DriverLibLinkWISR::readMultipleBytes(buffer, count);
#endif
}



void SlaveRegisterLayer::write(unsigned int registerAddress, unsigned const char * const buffer, unsigned int count) {
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
    DriverLibLinkWISR::writeMultipleBytes(bufferWithRegAddress, count+1);
}
