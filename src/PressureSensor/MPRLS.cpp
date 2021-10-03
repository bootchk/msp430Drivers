
#include "MPRLS.h"

// Use transport layer interface to I2C
#include "../../src/i2c/transport/i2cTransport.h"


/*
    setup and initialize communication with the hardware
    @param i2c_addr The I2C address for the sensor (default is 0x18)
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults
   to &Wire
    @returns True on success, False if sensor not found
*/

bool
MPRLS::begin(uint8_t i2c_addr) {
    // Init peripheral
    I2CTransport::initI2CPeripheral(i2c_addr);
    I2CTransport::configurePinsWithExternalPullups();
    I2CTransport::enable();


  // allow time for device to start
  __delay_cycles(1000000);
  // delay(10);

  // ensure device is active
  return ((readStatus() & MPRLS_STATUS_MASK) == MPRLS_STATUS_POWERED);
}

uint8_t
MPRLS::readStatus(void) {
  uint8_t buffer;

  // i2c_dev->read(buffer, 1);
  I2CTransport::read(0x0, buffer, 1);

  return buffer;
}


