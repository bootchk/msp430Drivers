
#include "MPRLS.h"

// Use transport layer interface to I2C
#include "../../src/i2c/transport/i2cTransport.h"

#include "../../src/assert/myAssert.h"




void
MPRLS::begin(uint8_t i2c_addr) {
    // Init peripheral
    I2CTransport::initI2CPeripheral(i2c_addr);
    I2CTransport::configurePinsWithExternalPullups();
    I2CTransport::enable();

    myAssert(I2CTransport::isInitialized);
    myAssert(I2CTransport::isConfiguredPinsForModule());
    myAssert(I2CTransport::isEnabled());

    /*
     *  Caller must allow time for device to come out of power on reset..
     *  Datasheet says one ms before first command.
     *
     *  The above only configures the master and does not talk to the device.
     */
}


/*

 WIP
  // ensure device is active
  // uint8_t status = readStatus();
  return true;
  // ((status & MPRLS_STATUS_MASK) == MPRLS_STATUS_POWERED);

   */



void
waitForDataReady(void) {
    // Option 1 wait for busy flag in status to clear
    // TODO

    // Option 2 wait for 5 ms
    __delay_cycles(5000);

}


uint32_t
MPRLS::readData(void) {
    // Initialize buffer all bytes zero
    // Same buffer used for write and read
    uint8_t buffer[4] = {MPRLS_MEASURE_COMMAND, 0, 0, 0};

    // Write command to wake device out of standby and begin measuring.
    // Send command and two bytes of the buffer, not all.
    // The two zero bytes are command parameters, not used by the device.
    I2CTransport::write(MPRLS_MEASURE_COMMAND, buffer, 2);

    waitForDataReady();

    // Read status byte and 3 bytes data
    I2CTransport::readWORegister(buffer, 4);

    // check status byte
    if (   (buffer[0] & MPRLS_STATUS_MATHSAT )
        || (buffer[0] & MPRLS_STATUS_FAILED  ) ) {
        return 0xFFFFFFFF;
    }
    else {
        // all good

        uint32_t result =
                (uint32_t(buffer[1]) << 16) |
                (uint32_t(buffer[2]) << 8)  |
                (uint32_t(buffer[3])          );
        return result;
    }
}



uint8_t
MPRLS::readStatus(void) {
  uint8_t buffer[1];

  I2CTransport::readWORegister(buffer, 1);

  return buffer[0];
}


uint32_t
MPRLS::readRawPressure(void) {
    return readData();
}

