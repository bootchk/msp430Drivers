
#include "lowSideSwitch.h"

#include "../../board.h"



void
LowSideSwitch::configure() {
    /* configure GPIO pin according to board.h */
    GPIO_setAsOutputPin(LOW_SIDE_SWITCH_PORT, LOW_SIDE_SWITCH_PIN);

    turnOff();
}


void
LowSideSwitch::turnOn() {
    // high to gate of NMOS FET opens the switch
    GPIO_setOutputHighOnPin(LOW_SIDE_SWITCH_PORT, LOW_SIDE_SWITCH_PIN);
}



void
LowSideSwitch::turnOff() {
    GPIO_setOutputLowOnPin(LOW_SIDE_SWITCH_PORT, LOW_SIDE_SWITCH_PIN);
}
