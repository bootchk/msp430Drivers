
#include "i2cPins.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>   // depends on msp430.h

#include <board.h>



void I2CPins::configure() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SDA_PORT,   I2C_SDA_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SCL_PORT,   I2C_SCL_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);
}

// TODO unconfigure ???
