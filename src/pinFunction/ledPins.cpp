
#include "ledPins.h"

#include "../board.h"




void LEDPins::configure() {
    // set value before configuring

#ifdef LAUNCHPAD_BOARD
    // Launch pad LED pins 1.0 and 1.1 (out)

    // TODO call driver functions for sink/source
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Test pin (green led) low
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);

#else
    // TODO depend on LEDAndLightSensor method??
    // both LED pins low
    GPIO_setOutputLowOnPin(NSideLEDPort, NSideLEDPin);
    GPIO_setOutputLowOnPin(PSideLEDPort, PSideLEDPin);

    // both LED pins initially output
    GPIO_setAsOutputPin(NSideLEDPort, NSideLEDPin);
    GPIO_setAsOutputPin(PSideLEDPort, PSideLEDPin);

#endif
}
