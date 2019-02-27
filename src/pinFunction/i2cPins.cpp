
#include "i2cPins.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>   // depends on msp430.h

#include <board.h>


/*
 * I2C is open collector bus design.
 * Thus requires a pullup on each signal, somewhere.
 */

void I2CPins::configureWithInternalPullup() {

    /*
     * Internal pullups   30k on MSP430FR2433
     * Here we assume the PCB does not have a discrete pullup.
     *
     * !!!! This is questionable, even for 100kbps bus rate.
     * It may work if bus capacitance is < 50 pF, i.e. for short bus and only master and slave device.
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


/*
 * From DriverLib example.  !!! InputPin, not OutputPin
 */
void I2CPins::configureWithExternalPullup() {
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
