
#include "ledAndLightSensor.h"

// DriverLib
#include <gpio.h>

// App
#include <board.h>




/*
 * Methods that use DriverLib and board.h
 * Some are public, most are private.
 */




/*
 * Functions on the P side pin (the measuring side.)
 */

bool LEDAndLightSensor::isLEDNPinLow() {
    unsigned char value;

    value = GPIO_getInputPinValue(NSideLEDPort, NSideLEDPin);
    /// assert value is 0 or 1
   return (  value == GPIO_INPUT_PIN_LOW );
}


void LEDAndLightSensor::enableHighToLowInterruptFromLEDNPin() {
    // require configured input

    /*
     * If pin already low, this sets IFG.  See "Interrupt Edge Select Registers"
     */
    GPIO_selectInterruptEdge(NSideLEDPort, NSideLEDPin, GPIO_HIGH_TO_LOW_TRANSITION);

    /*
     * Since GIE is not enabled, will not interrupt.
     */
    GPIO_enableInterrupt(NSideLEDPort, NSideLEDPin);
}

void LEDAndLightSensor::disableLEDNPinInterrupt() { GPIO_disableInterrupt(NSideLEDPort, NSideLEDPin); }

void LEDAndLightSensor::clearLEDNPinInterruptFlag() { GPIO_clearInterrupt(NSideLEDPort, NSideLEDPin); }






/*
 * State transitions.

 * States are: Off, On, ReverseBiased, Measuring
 */

void LEDAndLightSensor::toOffFromUnconfigured() {
    // Set both sides low before configuring direction out
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);

    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(PSideLEDPort,    PSideLEDPin);
    // assert N,P low output
}


void LEDAndLightSensor::toOnFromOff() {
    // assert off:
    // N,P low out
    GPIO_setOutputHighOnPin(PSideLEDPort,    PSideLEDPin);
    // assert on
    // N low out
    // P high out
}


void LEDAndLightSensor::toOffFromOn() {
    // N low out
    // P high out
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);
    // assert off:
    // N,P low out
}


void LEDAndLightSensor::toOffFromMeasuring() {
    // assert measuring:
    // N is unknown(low or high) input
    // P low output
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    // assert off:
    // N, P low output
}


void LEDAndLightSensor::toReversedFromOff() {
    // N low output
    // P low output
    GPIO_setOutputHighOnPin(NSideLEDPort,    NSideLEDPin);
    // assert reverse biased:
    // N high output
    // P low output
}


void LEDAndLightSensor::toMeasuringFromReversed() {
    // assert P,N outputs
    // assert P low, N high (reversed)

    // !!! Note we don't switch N to low which would start bleeding it

    // More or less equivalent to hardcoded:   P1DIR &= ~BIT7;
    GPIO_setAsInputPin(NSideLEDPort,    NSideLEDPin);

    // assert measuring:
    // P low output
    // N probably high, input (out value is also high, but it is now an input)
    // conceivably N could be low already
}
