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


void I2CDirect::initI2CPeripheral(unsigned int slaveAddress) {
    // Works.  Code from TI.
    UCB0CTLW0 |= UCSWRST;  // disable
    /*
     * UCMODE_3 => I2C
     * UCMST => master
     * UCSYNC => synchronous
     */
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    UCB0BRW = 160;                            // fSCL = SMCLK/160 = ~100kHz
    UCB0I2CSA = slaveAddress;                   // Slave Address
    UCB0IE |= UCNACKIE;     // lkk why enable NACK interrupt?
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
}
