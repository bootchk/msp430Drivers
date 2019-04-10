
#include "ledAndLightSensor.h"

// DriverLib
#include <gpio.h>

// App
#include <board.h>




/*
 * Methods that use DriverLib and board.h
 * Some are public, most are private.
 */

bool LEDAndLightSensor::isLow() {
    unsigned char value;

    value = GPIO_getInputPinValue(NSideLEDPort, NSideLEDPin);
    /// assert value is 0 or 1
   return (  value == GPIO_INPUT_PIN_LOW );
}


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
    // assert N,P output
    // assert N,P low
}


void LEDAndLightSensor::toOnFromOff() {
    // assert N,P are out
    // assert N,P are low
    GPIO_setOutputHighOnPin(PSideLEDPort,    PSideLEDPin);
    // assert N low, P high
}


void LEDAndLightSensor::toOffFromOn() {
    // assert N,P are out
    // assert N low, P high
    GPIO_setOutputLowOnPin(PSideLEDPort,    PSideLEDPin);
    // assert N,P low
}


void LEDAndLightSensor::toOffFromMeasuring() {
    // assert measuring:
    // N is high but dir is input
    // P low
    // P output
    GPIO_setOutputLowOnPin(NSideLEDPort,    NSideLEDPin);
    GPIO_setAsOutputPin(NSideLEDPort,    NSideLEDPin);
    // assert N,P low
    // assert N,P output
}


void LEDAndLightSensor::toReversedFromOff() {
    // assert N,P output
    // assert N,P low
    GPIO_setOutputHighOnPin(NSideLEDPort,    NSideLEDPin);
    // assert N high, P low (reverse biased)
}


void LEDAndLightSensor::toMeasuringFromReversed() {
    // assert P,N outputs
    // assert P low, N high (reversed)

    // !!! Note we don't switch N to low which would start bleeding it

    // More or less equivalent to hardcoded:   P1DIR &= ~BIT7;
    GPIO_setAsInputPin(NSideLEDPort,    NSideLEDPin);

    // assert P output
    // assert N input
    // assert P low (N value is also high, but it is an input)
}
