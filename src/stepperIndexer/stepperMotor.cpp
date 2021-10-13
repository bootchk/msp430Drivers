
#include "stepperMotor.h"

#include "stepperIndexer.h"
#include "chipInterface/chipInterface.h"



void
StepperMotor::wakeTurnAndSleep(unsigned int steps, MotorDirection direction) {

    StepperIndexer::wake();
    DriverChipInterface::setDirection(direction);
    DriverChipInterface::enableCoilDrive();

    // assert each step has delay to ensure motion to next step

    // First step allow for acceleration
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);

    for (int i = steps-1; i>0; i--) {
        StepperIndexer::stepDetentMaxSpeed();
    }

    DriverChipInterface::disableCoilDrive();
    StepperIndexer::delayForSettling();

    // Delay for deceleration and settling to detent
    // TODO
    StepperIndexer::sleep();
}
