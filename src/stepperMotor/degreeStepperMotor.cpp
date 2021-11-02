
#include "degreeStepperMotor.h"

#include "../../motor.h"

// Implementation
#include "stepperMotor.h"
#include "../stepperIndexer/chipInterface/chipInterface.h"
#include "../stepperIndexer/stepperIndexer.h"





void
DegreeStepperMotor::turnAndHoldMinDegrees(MotorDirection direction, MotorSpeed speed)
{
    StepperMotor::turnStepsDirectionSpeedAndHold(1, direction, speed);
}




void
DegreeStepperMotor::turnAndHoldDegrees(
        unsigned int degrees,
        MotorDirection direction,
        MotorSpeed speed)
{
    // assert degrees < 360

    /*
     * May be floating point math.
     *
     * TODO multiply by 10 so is always integer division
     */
    int steps = degrees / DEGREES_PER_STEP;

    StepperMotor::turnStepsDirectionSpeedAndHold(steps, direction, speed);
}


// TODO this is for 20 step motor
void
DegreeStepperMotor::turnAcceleratedQuarterRevAndHold(MotorDirection direction) {

    DriverChipInterface::setDirectionAndRelease(direction);
    DriverChipInterface::enableCoilDrive();

    // Steps accelerate and decellerate
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Quarter);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Max);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Quarter);

    StepperIndexer::delayForSettling();
}

#ifdef OLD
void
DegreeStepperMotor::wakeTurnAcceleratedQuarterRevAndSleep(MotorDirection direction) {
    StepperIndexer::wake();

    turnAcceleratedQuarterRevAndHold(direction);

    DriverChipInterface::disableCoilDrive();
    StepperIndexer::sleep();
}
#endif



/*
 * Simple delegation.
 */


void
DegreeStepperMotor::findPhysicalStopAndHold(
        MotorDirection direction,
        MotorSpeed     speed )
{
    // StepperMotor will attempt turn one rev i.e. 360 degrees
    StepperMotor::findPhysicalStopAndHold(direction, speed);
}

void
DegreeStepperMotor::delayUntilDriverChipAwake() {
    DriverChipInterface::delayForWake();
}

