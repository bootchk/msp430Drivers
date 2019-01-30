
#include "stepperDriver.h"

#include "board.h"

// DriverLib
#include <gpio.h>





#ifdef USE_STEPPER

void StepperDriver::turnPowerOn() {
    GPIO_setOutputHighOnPin(StepperDrivePowerSwitchPort,    StepperDrivePowerSwitchPin);
    //P1OUT |= DrivePowerSwitch;
    }

void StepperDriver::turnPowerOff() {
    //P1OUT &= ~DrivePowerSwitch;
    GPIO_setOutputLowOnPin(StepperDrivePowerSwitchPort,    StepperDrivePowerSwitchPin);
    }


#endif
