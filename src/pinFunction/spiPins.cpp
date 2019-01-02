
#include "spiPins.h"

// mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>   // depends on msp430.h

#include <board.h>



void SPIPins::configure() {
    // Data pins are part of the module
    configureDataSPIPins();
    // Slave select pins is outside of module, just a GPIO
    configureSelectSPIPin();
}


/*
 * Called when the module is disabled.
 *
 * For data pins:
 * Make the GPIO pin functions equal "GP" (not primary module).
 * One strategy is all pins output, which is low power and avoids all possibility of floating inputs.
 * Another strategy is leave the MISO pins as input, since slave should be driving it some value.
 *
 * Assert slave select pin is already configured direction out, value "not slave selected"
 */
void SPIPins::unconfigure() {

    GPIO_setAsOutputPin(MOSI_PORT,    MOSI_PIN);
    GPIO_setAsOutputPin(SPI_CLK_PORT, SPI_CLK_PIN);

    // OLD GPIO_setAsOutputPin(MISO_PORT,    MISO_PIN);
    GPIO_setAsInputPinWithPullDownResistor(MISO_PORT,    MISO_PIN);
}



void SPIPins::selectSlave(){
    GPIO_setOutputLowOnPin(RTCSelectPort, RTCSelectPin);
}
void SPIPins::deselectSlave(){
    GPIO_setOutputHighOnPin(RTCSelectPort, RTCSelectPin);
}



// Public because RTC is always alive so pin must remain configured during sleep

void SPIPins::configureSelectSPIPin() {
    /*
     * PxOut resets to undefined.
     * Set value high (not selected) before we enable pin.
     * This also chooses a pull direction if it is already configured as input (on reset.)
     */
    SPIPins::deselectSlave();
    GPIO_setAsOutputPin(RTCSelectPort, RTCSelectPin);
}

// Private


void SPIPins::configureDataSPIPins() {
    /*
     * See Datasheet, Table 6.19 (IO Diagrams) for ordinal of module.
     *
     * The SEL bits:
     * 00 == GPIO
     * 01 == primary module
     * 10 == secondary module
     *
     * On Port2, UCA1 is primary module
     */
    GPIO_setAsPeripheralModuleFunctionOutputPin(MOSI_PORT,   MOSI_PIN,     GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_CLK_PORT, SPI_CLK_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    // One input pin
    GPIO_setAsPeripheralModuleFunctionInputPin(MISO_PORT,     MISO_PIN,    GPIO_PRIMARY_MODULE_FUNCTION);
}
