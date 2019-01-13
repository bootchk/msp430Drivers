
#include "i2cPins.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>   // depends on msp430.h

#include <board.h>



void I2CPins::configure() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SDA_PORT,   I2C_SDA_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_SCL_PORT,   I2C_SCL_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);

    /*
     * pullups   30k on MSP430FR2433
     *
     * I2C requires a pullup somewhere.  Here we assume the PCB does not have one.
     *
     * !!! Does not change the pin function to general purpose, but does add the pullup.
     */
    GPIO_setAsInputPinWithPullUpResistor(I2C_SDA_PORT,   I2C_SDA_PIN);
    GPIO_setAsInputPinWithPullUpResistor(I2C_SCL_PORT,   I2C_SCL_PIN);
}

void I2CPins::unconfigure() {
    GPIO_setAsOutputPin(I2C_SDA_PORT,   I2C_SDA_PIN);
    GPIO_setAsOutputPin(I2C_SCL_PORT,   I2C_SCL_PIN);

    /*
     * TODO comments about why output pins have no pullup.
     */
}
