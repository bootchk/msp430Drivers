
#include "stepperMotor.h"

#include "stepperIndexer.h"
#include "chipInterface/chipInterface.h"

#include "../assert/myAssert.h"
#include "../SoC/SoC.h"


void
StepperMotor::wakeTurnAndSleep(
        unsigned int   steps,
        MotorDirection direction) {

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

void
StepperMotor::turnAndHold(
        unsigned int   steps,
        MotorDirection direction) {

    DriverChipInterface::setDirection(direction);
    DriverChipInterface::enableCoilDrive();

    // assert each step has delay to ensure motion to next step

    // First step allow for acceleration
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);

    for (int i = steps-1; i>0; i--) {
        StepperIndexer::stepDetentMaxSpeed();
    }

#ifdef TODO
    Failed to get a fault on locked rotor.
    Maybe the current limit can't be set to accomodate.
    When the current limit is low, the motor fails to turn.

    SoC::enableGlobalInterrupts();
    if (StepperIndexer::isFault())
        myAssert(false);
#endif

    StepperIndexer::delayForSettling();
}


// TODO hardcoded for 20 step motor
void
StepperMotor::wakeTurnAcceleratedQuarterRevAndSleep(MotorDirection direction) {
    StepperIndexer::wake();

    turnAcceleratedQuarterRevAndHold(direction);

    DriverChipInterface::disableCoilDrive();
    StepperIndexer::sleep();
}


void
StepperMotor::turnAcceleratedQuarterRevAndHold(MotorDirection direction) {

    DriverChipInterface::setDirection(direction);
    DriverChipInterface::enableCoilDrive();

    // Steps accelerate and decellerate
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Quarter);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Max);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Quarter);

    StepperIndexer::delayForSettling();
}


bool
StepperMotor::isFault() {
    return StepperIndexer::isFault();
}
