
/*
 * Implementation of public methods.
 * See ...Private.cpp for private methods.
 */


#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "stepperIndexer.h"

// Uses chip driver
#include "motor.h"

#include "../../src/delay/delay.h"
#include "../../src/assert/myAssert.h"




void StepperIndexer::syncDriverWithMotor() {
    // reset driver chip
    DriverChipInterface::sleep();

    // wake without maintaining shadows
    DriverChipInterface::wake();
    // assert DriverChip in home state (microstep 2 for Half StepMode)
    // assert motor is on unknown step, but shadowStepOfMotor is also HomeStep == 2

    // enough steps for motor to catch.  Two complete cycles through whole steps
    // This is for a four whole step motor
    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);

    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);
    StepperIndexer::stepDetentWithDelay(100);

    // assert motor in sync with microstep
    // assert driver chip in home state (microstep 2 for Half StepMode)
    // assert microstep is a DetentStep

    IndexerChipState::setMicrostepState(2);
}





/*
 * Stepping
 */

void
StepperIndexer::stepDetentWithDelay(unsigned int milliseconds) {
    /*
     * !!! Call DriverChipInterface.  Self controls speed.
     */
#if STEPPER_HARD_STEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
#elif STEPPER_HARD_STEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
#endif
}


void
StepperIndexer::stepManyDetents(unsigned int stepCount) {
    for (unsigned int i = stepCount; i>0; i--) {
        // TODO without calling this, we don't maintain shadowstep
        // stepDetent();
        // TODO temporarily avoid call overhead
#if STEPPER_HARD_STEP_SIZE_FULL
        // Each microstep is one full detentstep
        DriverChipInterface::stepMicrostep();
#endif
    }
}


void
StepperIndexer::stepDetentMaxSpeed() {
    /*
     * This is to move motor, require coils enabled.
     */
    myAssert(IndexerChipState::isCoilsEnabled());

    /*
     *  Call to  DriverChipInterface.
     *  Delay for each microstep to control speed.
     *  If don't delay between microsteps, the second step is lost.
     */
#if STEPPER_HARD_STEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    delayForSpeed(MotorSpeed::Max);

#elif STEPPER_HARD_STEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
#else
#warning "unhandled"
#endif
}


void
StepperIndexer::stepDetentAtSpeed(MotorSpeed speed) {
    /*
     * This is to move motor, require coils enabled.
     */
    myAssert(IndexerChipState::isCoilsEnabled());

    switch(speed) {
        case MotorSpeed::Max:
            stepDetentMaxSpeed();
            break;
        case MotorSpeed::Half:
            stepDetentMaxSpeed();
            stepDetentMaxSpeed();
            break;
        case MotorSpeed::Quarter:
            stepDetentMaxSpeed();
            stepDetentMaxSpeed();
            stepDetentMaxSpeed();
            stepDetentMaxSpeed();
            break;
        default:
            myAssert(false);
        }

}

#ifdef OLD
    /*
     *  Call to  DriverChipInterface.
     *  Delay for each microstep to control speed.
     *  If don't delay between microsteps, the second step is lost.
     */
#if STEPPER_HARD_STEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    delayForSpeed(MotorSpeed::Half);

#elif STEPPER_HARD_STEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
#else
#warning "unhandled"
#endif
}
#endif









/*
 * All delays are multiples of max delay,
 * since all speeds are fractions of max speed.
 */
void
StepperIndexer::delayForSpeed(MotorSpeed speed) {
    switch(speed) {
    case MotorSpeed::Max:
        delayForMaxSpeed();
        break;
    case MotorSpeed::Half:
        delayForMaxSpeed();
        delayForMaxSpeed();
        break;
    case MotorSpeed::Quarter:
        delayForMaxSpeed();
        delayForMaxSpeed();
        delayForMaxSpeed();
        delayForMaxSpeed();
        break;
    default:
        myAssert(false);
    }
}




void
StepperIndexer::delayForMaxSpeed() {

    // TODO for the defined max PPS instead of the motor
    /*
     * Might be 670 ???
     * By experiment, 500 PPS loses track and 250PPS does not.
     */
#if MOTOR_SYMBOL_TECH
#if STEPPER_HARD_STEP_SIZE_FULL
    StepperIndexer::delayFor250PPS();
#elif STEPPER_HARD_STEP_SIZE_HALF
    StepperIndexer::delayFor500PPS();
#else
    error
#endif


#elif MOTOR_SOYO_NIDEC
    StepperIndexer::delayFor6000PPS();
#else
#warning
#endif
}



/*
 TODO hard delays, or delays in terms of PPS???
    // one mS is not enough
    // ten mS
    Delay::tenMilliseconds();
    //Delay::hundredMilliseconds();
    // Delay::oneSecond();
     *
     * OR?? Settling speed should depend on prior speed
 */
void
StepperIndexer::delayForSettling() {
    //
    delayForSpeed(MotorSpeed::Half);
}



