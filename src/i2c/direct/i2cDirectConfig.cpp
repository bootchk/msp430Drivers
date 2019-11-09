/*
 * Config part of I2CDirect
 */


#include "i2cDirect.h"

#include "msp430.h"




void I2CDirect::configurePinsWithInternalPullups() {
    // Works.  Code from TI.

    // I2C pins
    // Port 1 pins 2 and 3 the primary module function
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);

    // Pullup internal (30k ohm)
    // No external pullups
    // I2C is open-collector, requires pull
    // REN, OUT, == 1 and DIR ==0 means input, pull enabled, pull is up
    P1OUT |= BIT2 | BIT3;
    P1REN |= BIT2 | BIT3;
    P1DIR &= ~(BIT2 | BIT3);    // bit clear
}


void I2CDirect::configurePinsWithExternalPullups() {
    // P1.2 UCB0 SDA data
    // P1.3 UCB0 SCL clock

    // Assume that DIR is irrelevant

    // Primary module function selected:  0x01 SEL1 == 0, SEL0 == 1
    P1SEL0 |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);
}



void I2CDirect::initI2CPeripheral(unsigned int slaveAddress) {
    // Works.  Code from TI.
    UCB0CTLW0 |= UCSWRST;  // disable
    /*
     * UCMODE_3 => I2C
     * UCMST => master
     * UCSYNC => synchronous
     */
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    //UCB0BRW = 10;                            // fSCL = SMCLK/10 = 1Mhz/10 = ~100kHz
    UCB0BRW = 100;                            // fSCL = SMCLK/100 = 1Mhz/10 = ~10kHz
    UCB0I2CSA = slaveAddress;                 // Slave Address

    configurePinsWithExternalPullups();

    UCB0IE |= UCNACKIE;                       // lkk why enable NACK interrupt?
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
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
    configurePinsWithExternalPullups();

    // OR
    //I2CPins::configureWithInternalPullup();


    // enable
    UCB0CTLW0 &= ~UCSWRST;

    // insure device configured and enabled
    // does not configure pins
}
