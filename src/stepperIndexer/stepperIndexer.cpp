
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
    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);

    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);
    StepperIndexer::stepDetent(100);

    // assert motor in sync with microstep
    // assert driver chip in home state (microstep 2 for Half StepMode)
    // assert microstep is a DetentStep

    StepperIndexer::setShadowMicrostepOfDriver(2);
}





/*
 * Stepping
 */

void
StepperIndexer::stepDetent(unsigned int milliseconds) {
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

    maintainShadowStep();
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
    // TODO maintain shadow
}


void
StepperIndexer::stepDetentMaxSpeed() {
    myAssert(IndexerChipState::isCoilsEnabled());

    /*
     *  Call to  DriverChipInterface.
     *  Delay for each microstep to control speed.
     *  If don't delay between microsteps, the second step is lost.
     */
#if STEPPER_HARD_STEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();

#elif STEPPER_HARD_STEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayForMaxSpeed();
#else
#warning "unhandled"
#endif

    maintainShadowStep();
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

