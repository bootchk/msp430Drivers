
#include "i2cPins.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>   // depends on msp430.h

#include <board.h>



void I2CPins::configure() {

    /*
     * Internal pullups   30k on MSP430FR2433
     *
     * I2C requires a pullup on each signal, somewhere.  Here we assume the PCB does not have a discrete pullup.
     *
     * Modifies PxDIR to "input" PxOUT to "pullup" and PxREN to "enable pulllup"
     * !!! This also changes PxSEL (changesthe pin function to general purpose)
     * Thus must follow: change function to module primary
     */
    GPIO_setAsInputPinWithPullUpResistor(I2C_SDA_PORT, I2C_SDA_PIN);
    GPIO_setAsInputPinWithPullUpResistor(I2C_SCL_PORT, I2C_SCL_PIN);

    /*
     * Modifies PxDIR to "input" and PxSel to "Primary Module"
     */
    // Was Output
    GPIO_setAsPeripheralModuleFunctionInputPin(I2C_SDA_PORT,   I2C_SDA_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(I2C_SCL_PORT,   I2C_SCL_PIN,  GPIO_PRIMARY_MODULE_FUNCTION);
}



void I2CPins::unconfigure() {
    /*
     * Set value high before converting to output.
     * Idle state of bus requires high.
     */
    GPIO_setOutputHighOnPin(I2C_SDA_PORT, I2C_SDA_PIN);
    GPIO_setOutputHighOnPin(I2C_SDA_PORT, I2C_SCL_PIN);

    GPIO_setAsOutputPin(I2C_SDA_PORT,   I2C_SDA_PIN);
    GPIO_setAsOutputPin(I2C_SCL_PORT,   I2C_SCL_PIN);

    /*
     * Why output pins have no pullup: see the AND gate on the GPIO.  If PxDIR == 1 (output), resistor cannot be active even if PxREN == 1.
     */
}
