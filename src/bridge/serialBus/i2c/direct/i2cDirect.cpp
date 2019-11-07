
#include "i2cDirect.h"


#include "board.h"  // instanceAddress, RTCBusAddress

//#include "../../../driverParameters.h"  // I2C bus speed, slaveAddress

// Driverlib
// This gets register addresses and bits
// No need for this if include DriverLib   #include "msp430.h"
#include <eusci_b_i2c.h>

// For configuration with internal pullup
#include "../../../../pinFunction/i2cPins.h"

void I2CDirect::readTime() {
    readFromAddress( 0x00 );   // Time is 8 bytes at address 0x0)
}


unsigned int I2CDirect::readID() {
    return readFromAddress( 0x28 );   // ID is 1 bytes at address 0x28
}





// wait for bus ready.  Insure not other master is holding bus by pulling down SDA while SCL high (start condition detected.)
void waitForBusReady() {

    // TEMP
    return;

   unsigned int flags;

   flags = UCB0STATW;
   while (UCB0STATW & UCBBUSY) {
        // Stop, to force other masters off bus
        UCB0CTLW0 |= UCTXSTP;

        // wait for stop complete
        while (UCB0CTLW0 & UCTXSTP)  ;
        // clear IFG by writing generator
        UCB0IV = 1;
   }
}




/*
 * This is a combined transaction: START, write, RESTART, read, STOP
 */
void
I2CDirect::readFromAddress(unsigned int registerAddress,
                unsigned char * const buffer,
                unsigned int count)
{
unsigned int countDown = count;
unsigned int bufferIndex = 0;

waitForBusReady();

UCB0CTLW0 |= UCTR;
UCB0CTLW0 |= UCTXSTT;

waitForStart();

// write register address (in RTC address space)
UCB0TXBUF = registerAddress;

// wait for driver to shift TXBUF to its internal buffer
while (!(UCB0IFG & UCTXIFG0)) ;

// restart for rx
UCB0CTLW0 &= ~UCTR;
UCB0CTLW0 |= UCTXSTT;

// Read first count-1 bytes
while (countDown > 1 ) {
    // wait for byte to be read
    while (!(UCB0IFG & UCRXIFG0)) ;
    countDown--;

    // copy byte from device buffer, side effect clear RXIFG
    buffer[bufferIndex] = UCB0RXBUF;
    bufferIndex++;
}
// assert counDown == 1

// stop after last byte
UCB0CTLW0 |= UCTXSTP;

while (!(UCB0IFG & UCRXIFG0)) ;
buffer[bufferIndex] = UCB0RXBUF;

// wait for stop complete
while (UCB0CTLW0 & UCTXSTP)  ;
}





void
I2CDirect::writeToAddress(unsigned int registerAddress, const unsigned char * const buffer, unsigned int count)
{
    unsigned int countDown = count;
    unsigned int bufferIndex = 0;

    // start for tx
    UCB0CTLW0 |= UCTR;
    UCB0CTLW0 |= UCTXSTT;

    while (!(UCB0IFG & UCTXIFG0)) ;

    // write register address (in RTC address space)
    UCB0TXBUF = registerAddress;


    while (count > 0 ) {
        // Wait for previous byte (register address or previous data byte)
        while (!(UCB0IFG & UCTXIFG0)) ;

        // write value to addressed register in slave
        UCB0TXBUF = buffer[bufferIndex];
        bufferIndex ++;
        countDown--;
    }
    // assert count == 0

    // stop
    UCB0CTLW0 |= UCTXSTP;

    // wait for stop complete
     while (UCB0CTLW0 & UCTXSTP)  ;
}


void
I2CDirect::writeToAddress(unsigned int registerAddress, unsigned char value) {
    // Address of value on stack
    readFromAddress(registerAddress, &value, 1);
}


/*
 * Single byte of data.
 * Uses multiple byte read, provides buffer.
 */
unsigned char
I2CDirect::readFromAddress(unsigned int registerAddress) {
    unsigned char dataByte;
    readFromAddress(registerAddress, &dataByte, 1);
    return dataByte;
}





#ifdef OLD
// requires init() and pins configured
unsigned int  I2CDirect::readFromAddress(unsigned int registerAddress) {
    unsigned int byte1, byte2, byte3;
    unsigned int flags;


        waitForBusReady();


        // start for tx
        // Was UCB0CTL1 for earlier family members
        // Both bits set at once??? UCB0CTLW0|= UCTR + UCTXSTT;
        UCB0CTLW0 |= UCTR;
        UCB0CTLW0 |= UCTXSTT;

        // wait for start and slave address done

#define WAIT_TXIFG
#ifdef WAIT_TXIFG
        // Some docs say TXIFG will be set
        // !!! bit is UCTXIFG in earlier family members, note suffix 0
        while (!(UCB0IFG & UCTXIFG0)) {
            flags = UCB0IFG;
        }
#else
        // Some docs say TXSTT will be cleared
        while (UCB0CTLW0 & UCTXSTT) ;

        // Some docs say to check both TXSTT and TXIFG
#endif

        flags = UCB0IFG;

        // write register address (in RTC address space)
        UCB0TXBUF = registerAddress;

        // wait for driver to shift TXBUF to its internal buffer
        while (!(UCB0IFG & UCTXIFG0)) {
                    flags = UCB0IFG;
                }

        // restart for rx
        UCB0CTLW0 &= ~UCTR;
        UCB0CTLW0 |= UCTXSTT;

        // wait for first byte to be read
        while (!(UCB0IFG & UCRXIFG0)) {
            flags = UCB0IFG;
        }

        // copy byte from device buffer, side effect clear RXIFG
        byte1 = UCB0RXBUF;

        while (!(UCB0IFG & UCRXIFG0)) ;
        byte2 = UCB0RXBUF;

        // stop after next byte
        UCB0CTLW0 |= UCTXSTP;

        while (!(UCB0IFG & UCRXIFG0)) ;
        byte3 = UCB0RXBUF;

        // wait for stop complete
        while (UCB0CTLW0 & UCTXSTP)  ;

        // insure buffer was read and ready for next transaction
        return byte1;
}

#endif







void I2CDirect::setSlaveAddress(unsigned int slaveAddress) {
    // right justified 7-bit address
    UCB0I2CSA = slaveAddress;
}


void I2CDirect::init(void)
{
    // disable
    UCB0CTLW0 |= UCSWRST;

    //  I2C mode, master mode, clock is SMCLK, UCSYNC (required for I2C)
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC;

    // bit rate divisor
    //UCB0BRW = 4;   // 250kHz at 1Mhz SMCLK
    UCB0BRW = 8;   // 125kHz at 1Mhz SMCLK

    // TI recommends configuring GPIO pins before enabling module

    // Configure directly, but with external pullups
    configurePins();

    // OR
    //I2CPins::configureWithInternalPullup();


    // enable
    UCB0CTLW0 &= ~UCSWRST;

    // insure device configured and enabled
    // does not configure pins
}


void I2CDirect::configurePins() {
    // P1.2 UCB0 SDA data
    // P1.3 UCB0 SCL clock

    // Assume that DIR is irrelevant

    // Assume external pulllups

    // TODO configure internal pullups


    // Primary module function selected:  0x01 SEL1 == 0, SEL0 == 1
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);
}
