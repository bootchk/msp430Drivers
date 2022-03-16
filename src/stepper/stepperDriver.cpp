
#include "stepperDriver.h"

#include "board.h"

#include <src/loadSwitch/loadSwitch.h>



#ifdef USE_STEPPER

void StepperDriver::turnPowerOn() {
    HighSideSwitch::turnOn();

    // CRUFT
    // GPIO_setOutputHighOnPin(StepperDrivePowerSwitchPort,    StepperDrivePowerSwitchPin);
    // P1OUT |= DrivePowerSwitch;
    }

void StepperDriver::turnPowerOff() {
    HighSideSwitch::turnOn();

    // CRUFT
    //P1OUT &= ~DrivePowerSwitch;
    // GPIO_setOutputLowOnPin(StepperDrivePowerSwitchPort,    StepperDrivePowerSwitchPin);
    }


#endif
