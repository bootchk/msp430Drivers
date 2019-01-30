
#include "stepperPins.h"

// DriverLib
#include <gpio.h>

#include "board.h"


#ifdef USE_STEPPER

void StepperPins::configureStepperCoilPins() {
    // All stepper pins are function: general purpose, direction: outputs

    // Coils A and B, each with terminals 1 and 2
    GPIO_setAsOutputPin(MotorCoilA1Port,    MotorCoilA1Pin);
    GPIO_setAsOutputPin(MotorCoilA2Port,    MotorCoilA2Pin);

    GPIO_setAsOutputPin(MotorCoilB1Port,    MotorCoilB1Pin);
    GPIO_setAsOutputPin(MotorCoilB2Port,    MotorCoilB2Pin);

    // TODO power
}

#endif
