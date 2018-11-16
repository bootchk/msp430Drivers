
#include "spi.h"

/*
 * Derived from
 * Maksim Gorev spi_driverlib.c http://www.breakmyboard.com/blog/ti-mcu-ti-driverlib-based-simple-spi-library/
 * https://github.com/machinaut/msp432-driverlib.git/spi.c
 * MSP430F5529_driverlib_examples/13A_USCI_B_SPI_MCP41010_digiPot
 * TI-RTOS for SimpleLink Wireless MCUs ... SPI.h
 *
 */
/*
 * Much specialization here, see TI docs
 * Serial
 *    USCI	(older device)
 *    eUSCI (newer device)
 *       eUSCI_A variant support UART, iRDA, SPI  <====== this
 *          one or more instances, e.g. A0 and A1
 *       eUSCI_B variant support I2C or SPI
 *          one or more instances per family member
 */



/*
 * To change to another instance, edit board.h and include different driverlib
 */
// TI DriverLib
#include "eusci_a_spi.h"
//#include "eusci_b_spi.h"

#include "gpio.h"

#include "../../pinFunction/spiPins.h"
#include "../../board.h"

#include "../../debug/myAssert.h"



void SPI::enable() {
	EUSCI_A_SPI_enable(SPIInstanceAddress);
}


void SPI::disable() {
	EUSCI_A_SPI_disable(SPIInstanceAddress);
}

bool SPI::isEnabled() {
    // implemented raw registers since Driverlib has no function
    // (bit0 == 1) => held in reset i.e. disabled
#ifdef USE_EUSCI_A0
    return ((UCA0CTLW0 & BIT0) == 0) ;
#else
    return ((UCA1CTLW0 & BIT0) == 0) ;
#endif
}


bool SPI::isInterrupt() {
    bool result;

    /// unsigned int foo = SPIInstanceAddress;
    result = EUSCI_A_SPI_getInterruptStatus(SPIInstanceAddress,
                                           (EUSCI_A_SPI_TRANSMIT_INTERRUPT | EUSCI_A_SPI_RECEIVE_INTERRUPT));
    return result;
}

void SPI::clearInterrupt() {
    EUSCI_A_SPI_clearInterrupt(SPIInstanceAddress,
                               (EUSCI_A_SPI_TRANSMIT_INTERRUPT | EUSCI_A_SPI_RECEIVE_INTERRUPT));
}


/*
 * A transfer is the master clocking the bus.
 * The slave may put data on MISO
 * and master put data on MOSI on each clock cycle.
 * I.E. duplex communication.
 */
unsigned char SPI::transfer(unsigned char value) {

    /*
     * Requires any previous transfer complete.
     * we wait afterwards, but for safety also check before.
     * Maybe configuration takes some time before SPI module is not busy?
     */
    while(EUSCI_A_SPI_isBusy(SPIInstanceAddress)) ;

	EUSCI_A_SPI_transmitData(SPIInstanceAddress, value);

	/*
	 * Spin until transfer is complete.
	 * SPI bus is slower than CPU.
	 * Wait until result (of duplex communication).
	 *
	 * Finite.  If infinite duration, hw has failed.
	 *
	 * Other implementations use an interrupt flag???
	 */
	while(EUSCI_A_SPI_isBusy(SPIInstanceAddress)) ;

	return EUSCI_A_SPI_receiveData(SPIInstanceAddress);
}

/*
 * Per eUSCI chapter of user guide,
 * configure when disabled, in this order
 */
void SPI::configureMaster() {
    myAssert(not isEnabled());
	configureMasterDevice();
	SPIPins::configure();
}

void SPI::unconfigureMaster() {
	/*
	 * Don't need to unconfigure SPI master (eUSCI), since disabled, is reset.
	 * Only unconfigure pins.
	 */
	SPIPins::unconfigure();
}







/*
 * Configure device.
 *
 * SPI bus has many variations.
 * Remote device usually hardwires one variation.
 * Configuration of master's SPI device must match that of remote device.
 *
 * Other config: clock source not dictated by SPI standard
 *
 * FUTURE: this could be done with a const struct
 */
void SPI::configureMasterDevice() {

	EUSCI_A_SPI_initMasterParam param = {0};

	// Other API's just setClockDivider()
	param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
	param.clockSourceFrequency = 8000000;
	/*
	 * I experienced failure in RTC comm, and tried to reduce bit rate.
	 * I also shortened wires.  May be noise, or loose connection.
	 */
	param.desiredSpiClock = 1000000;
	///param.desiredSpiClock = 500000;

	// setBitOrder()
	param.msbFirst = EUSCI_A_SPI_MSB_FIRST;

    // SPI clock mode
	// SPI_MODE == 0
	// setDataMode()
	param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
	param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;

	/*
	 * TI's SPI mode
	 * 3-pin mode (SCLK, SMOSI, and SMISO) w separate SS not part of eUSCI module
	 */
	param.spiMode = EUSCI_A_SPI_3PIN;

	EUSCI_A_SPI_initMaster(SPIInstanceAddress, &param);
}
