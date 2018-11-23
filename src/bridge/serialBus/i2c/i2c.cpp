
#include "i2c.h"

/*
 * Derived from
 * Maksim Gorev spi_driverlib.c http://www.breakmyboard.com/blog/ti-mcu-ti-driverlib-based-simple-spi-library/
 * https://github.com/machinaut/msp432-driverlib.git/spi.c
 * MSP430F5529_driverlib_examples/13A_USCI_B_I2C_MCP41010_digiPot
 * TI-RTOS for SimpleLink Wireless MCUs ... I2C.h
 *
 */
/*
 * Much specialization here, see TI docs
 * Serial
 *    USCI	(older device)
 *    eUSCI (newer device)
 *       eUSCI_A variant support UART, iRDA, I2C  <====== this
 *          one or more instances, e.g. A0 and A1
 *       eUSCI_B variant support I2C or I2C
 *          one or more instances per family member
 */



/*
 * To change to another instance, edit board.h and include different driverlib
 */
// TI DriverLib
#include "eusci_a_spi.h"
#include "eusci_b_i2c.h"
// alternate device: #include "eusci_b_spi.h"

// Configuration: I2CInstanceAddress
#include "../../../board.h"


// Configure pins used for I2C
#include "../../../pinFunction/spiPins.h"

// Configure mangling of address byte
#include "../../addressMangler.h"



//#include "../../debug/myAssert.h"



void I2C::enable() {
	EUSCI_B_I2C_enable(I2CInstanceAddress);
}


void I2C::disable() {
	EUSCI_B_I2C_disable(I2CInstanceAddress);
}

bool I2C::isEnabled() {
    // implemented raw registers since Driverlib has no function
    // (bit0 == 1) => held in reset i.e. disabled
#ifdef USE_EUSCI_B0
    return ((UCA0CTLW0 & BIT0) == 0) ;
#else
    return ((UCA1CTLW0 & BIT0) == 0) ;
#endif
}


/*
 * i2c has multiple interrupts per instance
 */
bool I2C::isInterrupt() {
    bool result;

    /// unsigned int foo = I2CInstanceAddress;
    result = EUSCI_B_I2C_getInterruptStatus(I2CInstanceAddress,
                                           (EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_RECEIVE_INTERRUPT0));
    return result;
}

void I2C::clearInterrupt() {
    EUSCI_B_I2C_clearInterrupt(I2CInstanceAddress,
                               (EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_RECEIVE_INTERRUPT0));
}


/*
 * A transfer is the master clocking the bus.
 * The slave may put data on MISO
 * and master put data on MOSI on each clock cycle.
 * I.E. duplex communication.
 */
unsigned char I2C::transfer(unsigned char value) {

    /*
     * Requires any previous transfer complete.
     * we wait afterwards, but for safety also check before.
     * Maybe configuration takes some time before I2C module is not busy?
     *
     * i2c has "BusBusy" name instead of "Busy"
     */
    while(EUSCI_B_I2C_isBusBusy(I2CInstanceAddress)) ;

	EUSCI_B_I2C_masterSendSingleByte(I2CInstanceAddress, value);

	/*
	 * Spin until transfer is complete.
	 * I2C bus is slower than CPU.
	 * Wait until result (of duplex communication).
	 *
	 * Finite.  If infinite duration, hw has failed.
	 *
	 * Other implementations use an interrupt flag???
	 */
	while(EUSCI_B_I2C_isBusBusy(I2CInstanceAddress)) ;

	return EUSCI_B_I2C_masterReceiveSingleByte(I2CInstanceAddress);
}




/*
 * Only configures 3 of the 4 pins (not the Slave Select pin):
 * MOSI, MISO, SCLK
 *
 * TI Energia document doesn't say this configure MISO ???
 */

/*
 * Per eUSCI chapter of user guide,
 * configure when disabled, in this order
 */
void I2C::configureMaster(bool isRWBitHighForRead) {
    // myAssert(not isEnabled());
	configureMasterDevice();
	RegisterAddressMangler::configureRWBitHighForRead(isRWBitHighForRead);

	// i2c does not have dedicated pins for select slave
}

void I2C::unconfigureMaster() {
	/*
	 * Don't need to unconfigure I2C master (eUSCI), since disabled, is reset.
	 * Don't need to  unconfigure pins.
	 */
}




static EUSCI_B_I2C_initMasterParam param {
    EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
    8000000,
    EUSCI_B_I2C_SET_DATA_RATE_400KBPS,
    16,
    EUSCI_B_I2C_NO_AUTO_STOP,
};


/*
 * Configure device on mcu (the master.)
 *
 * I2C bus has many variations.
 * Remote device usually hardwires one variation.
 * Configuration of master's I2C device must match that of remote device.
 *
 * Other config: clock source not dictated by I2C standard
 */
void I2C::configureMasterDevice() {
	EUSCI_B_I2C_initMaster(I2CInstanceAddress, &param);
}


/*
 *
 * In I2C, select slaves by transmit slave address over bus.
 *
 */
void I2C::selectSlave(unsigned int slave) { ; } // TODO }

void I2C::deselectSlave() { ; } // TODO }
