
#include "stepperMotor.h"

#include "../stepperIndexer/stepperIndexer.h"
#include "../stepperIndexer/chipInterface/chipInterface.h"
#include "../../motor.h"

#include "../assert/myAssert.h"
#include "../SoC/SoC.h"


void
StepperMotor::wake() {
    StepperIndexer::wake();
}


void
StepperMotor::delayUntilDriverChipAwake() {
    DriverChipInterface::delayForWake();
}


void
StepperMotor::wakeTurnAndSleep(
        unsigned int   steps,
        MotorDirection direction) {

    StepperIndexer::wake();
    DriverChipInterface::setDirectionAndRelease(direction);
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
StepperMotor::turnAndHoldAccelerated(
        unsigned int   steps,
        MotorDirection direction) {

    DriverChipInterface::setDirectionAndRelease(direction);
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

void
StepperMotor::turnAndHoldAtSpeed(
        unsigned int   steps,
        MotorDirection direction,
        MotorSpeed     speed
        )
{

    // Ensure coil driven, because otherwise step are ineffective
    DriverChipInterface::enableCoilDrive();

    DriverChipInterface::setDirectionAndHold(direction);

    // assert each step has delay to ensure motion to next step
    for (int i = steps; i>0; i--) {
        StepperIndexer::stepDetentAtSpeed(speed);
    }

    StepperIndexer::delayForSettling();
}


void
StepperMotor::turnAndHoldMicrosteps(
        unsigned int   microsteps,
        MotorDirection direction) {

    DriverChipInterface::setDirectionAndHold(direction);
    // assert little delay occurred and coils enabled

    // assert each microstep has delay to ensure motion to next step

    for (int i = microsteps; i>0; i--) {
        StepperIndexer::stepMicrostepMaxSpeed();
    }

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


void
StepperMotor::turnAcceleratedStepsAndHold(
        unsigned int   stepCount,
        MotorDirection direction) {

    DriverChipInterface::setDirectionAndRelease(direction);
    DriverChipInterface::enableCoilDrive();

    // Steps accelerate and decelerate

    // First step at half speed
    StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);

    for (unsigned int i=stepCount-2; i>0; i--) {
        // steps 2, n-1 at full speed
        StepperIndexer::stepDetentAtSpeed(MotorSpeed::Max);
    }

    if (stepCount > 1)
        StepperIndexer::stepDetentAtSpeed(MotorSpeed::Half);

    StepperIndexer::delayForSettling();
}


bool
StepperMotor::isFault() {
    return StepperIndexer::isFault();
}


void
StepperMotor::findPhysicalStopAndHold(MotorDirection direction) {


    StepperIndexer::wake();
    DriverChipInterface::setDirectionAndRelease(direction);
    DriverChipInterface::enableCoilDrive();

    // Turn one revolution (enough to encounter the physical stop.)
    StepperIndexer::stepManyDetentsAtSpeed(MOTOR_STEPS_PER_REV, MotorSpeed::Quarter);
}

void
StepperMotor::syncMotorWithModel() {

    // The datasheet is not clear that sleep will disable coil drive.
    DriverChipInterface::disableCoilDrive();
    /*
     * The stop should be such that no other forces drive the motor off the stop.
     * Otherwise, disableCoilDrive() will let those other forces move the motor off the stop.
     *
     * We disableCoilDrive() so the motor assumes the nearest undriven detent position.
     * But we don't want the motor to move past that.
     */

    // reset driver chip
    StepperIndexer::sleep();
    StepperIndexer::wake();

    // assert motor in sync with microstep
    // assert driver chip in home state (microstep 2 for Half StepMode)
    // assert microstep is a DetentStep

    IndexerChipState::setMicrostepState(2);
}


void
StepperMotor::jiggle() {
    /*
     * TODO
     * This assumes current direction is
     * TODO
     * This is for quarter microstep
     */
    StepperMotor::turnAndHoldMicrosteps(2, MotorDirection::Forward);
    StepperMotor::turnAndHoldMicrosteps(2, MotorDirection::Backward);
}
