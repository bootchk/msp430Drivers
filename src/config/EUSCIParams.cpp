#pragma once

// Driverlib
#include <eusci_b_i2c.h>

#include "driverParameters.h"  // I2C bus speed



/*
 * Struct of params passed to initialize DriverLib for EUSCI
 *
 *
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
