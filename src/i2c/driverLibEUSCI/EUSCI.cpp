
#include "EUSCI.h"

// Driverlib
#include <eusci_b_i2c.h>

#include "board.h"  // I2CInstanceAddress

#include "../../config/EUSCIParams.h"   // EUSCI_B_I2C_initMasterParam



void EusciDriver::initI2CPeripheral(unsigned int slaveAddress) {
    EUSCI_B_I2C_initMaster(I2CInstanceAddress,  &params);

    /*
     * initMaster is flawed re data rate.  See my TI forum posts.
     * So caller should set it explicitly.
     */

    EUSCI_B_I2C_setSlaveAddress(I2CInstanceAddress, slaveAddress);
}
