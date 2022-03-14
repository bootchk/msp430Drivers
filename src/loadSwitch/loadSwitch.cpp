
#include "loadSwitch.h"

#include "../../board.h"



void
HighSideSwitch::configure() {
    /* configure GPIO pin according to board.h */
    GPIO_setAsOutputPin(HIGH_SIDE_SWITCH_PORT, HIGH_SIDE_SWITCH_PIN);

    turnOff();
}


void
HighSideSwitch::turnOn() {
    // high to gate of NMOS FET opens the switch
    GPIO_setOutputHighOnPin(HIGH_SIDE_SWITCH_PORT, HIGH_SIDE_SWITCH_PIN);
}



void
HighSideSwitch::turnOff() {
    GPIO_setOutputLowOnPin(HIGH_SIDE_SWITCH_PORT, HIGH_SIDE_SWITCH_PIN);
}
