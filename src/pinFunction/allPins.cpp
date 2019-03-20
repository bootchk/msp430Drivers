
#include "allPins.h"

#include <msp430.h>

#include "board.h"  // AlarmSignalPin

#include "../assert/myAssert.h"

void AllPins::setOutput() {

    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;

#ifdef __MSP430FR6989__

    P4DIR = 0xFF;
    P5DIR = 0xFF;
    P6DIR = 0xFF;
    P7DIR = 0xFF;
    P8DIR = 0xFF;
    P9DIR = 0xFF;
    P10DIR = 0xFF;
    PJDIR = 0xFF;

#endif
}

void AllPins::setValue(unsigned char value) {

    P1OUT = value;
    P2OUT = value;
    P3OUT = value;

#ifdef __MSP430FR6989__
    P4OUT = value;
    P5OUT = value;
    P6OUT = value;
    P7OUT = value;
    P8OUT = value;
    P9OUT = value;
    P10OUT = value;
    PJOUT = value;
#endif
}


void AllPins::setHighOutput() {
    setOutput();
    setValue(0xFF);
}
void AllPins::setLowOutput() {
    setOutput();
    setValue(0);
}


bool AllPins::areGeneralPurpose() {
    return (P1SEL0 == 0) and (P1SEL1 == 0)
            and (P2SEL0 == 0) and (P2SEL1 == 0)
            and (P3SEL0 == 0) and (P3SEL1 == 0);
}


// TODO hardcoded for FR2433, my pin assignments
void AllPins::assertAreConfiguredForSleep() {


    // only alarm pin is input
    // Compiler complains about this equivalent: ???? (P1DIR == ~AlarmSignalPin);
    myAssert(P1DIR == 0xFD);

    myAssert(
        // all port1 pins (especially I2C bus pins) are high
        // except P1.5 and P1.7 (LED legs left low)
        // TODO make them left high for OFF state
        (P1OUT == 0x5F));

    /*
     * other ports output.
     * Value is undefined, especially after a reset.
     *
     * Experimentation shows that OUT does vary after a reset.
     */
    myAssert(
        (P2DIR == 0xFF) and

        // port 3 only 3 pins
        (P3DIR == 0x7)
    );
}
