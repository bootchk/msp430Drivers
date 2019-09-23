
#include "ledPins.h"

// board.h comes from App or from test harness directory
#include <board.h>



void LEDPins::configure() {
    // TODO depend on LEDAndLightSensor method??
    // IE this assumes driving both sides of LED

    // set value before configuring
    // both LED pins low
    GPIO_setOutputLowOnPin(NSideLEDPort, NSideLEDPin);
    GPIO_setOutputLowOnPin(PSideLEDPort, PSideLEDPin);

    // both LED pins initially output
    GPIO_setAsOutputPin(NSideLEDPort, NSideLEDPin);
    GPIO_setAsOutputPin(PSideLEDPort, PSideLEDPin);
}
