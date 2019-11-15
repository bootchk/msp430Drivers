/*
 * read and write methods of bridge
 */

#include "bridge.h"

#include "serialBus/serial.h"

#include "../assert/myAssert.h"

#ifdef BRIDGE_USE_SERIAL


void Bridge::write(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    myRequire(isConfigured());

    // TODO chosenDevice
    Serial::selectSlave(1);
    Serial::write(registerAddress, buffer, count);
    Serial::deselectSlave();
}


void Bridge::read(const RegisterAddress registerAddress,
                      unsigned char * const buffer,
                      const unsigned int count) {
    myRequire(isConfigured());

    Serial::selectSlave(1);
    Serial::read(registerAddress, buffer, count);
    Serial::deselectSlave();
}

#endif  // BRIDGE_USE_SERIAL
