
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






 // configure all GPIO out to ensure low power
void
AllPins::configureGPIOLowPower() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3.
    // Datasheet says only 3 pins??
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
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

void
AllPins::setAllOutputsLow() {
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;
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
    return      (P1SEL0 == 0) and (P1SEL1 == 0)
            and (P2SEL0 == 0) and (P2SEL1 == 0)
            and (P3SEL0 == 0) and (P3SEL1 == 0);
}


// pin assignments hardcoded for FR2433, embeddedDutyCycle framework, SmartBlinker app
void AllPins::assertAreConfiguredForSleep() {


    // only alarm pin is input
    // Compiler complains about this equivalent: ???? (P1DIR == ~AlarmSignalPin);
    myAssert(P1DIR == 0xFD);

    // all port1 pins (especially I2C bus pins) are high.
    // except when app uses P1.5 and P1.7 (LED legs outputs, value low)
    // (P1OUT == 0x5F));
    myAssert( P1OUT == 0xFF );

    /*
     * other ports output.
     * Value is undefined, especially after a reset.
     *
     * Experimentation shows that OUT does vary after a reset.
     */
    myAssert(
        (P2DIR == 0xFF) and
        (P3DIR == 0x7)  // port 3 only 3 pins
    );
}
