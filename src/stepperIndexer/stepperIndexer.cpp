
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
    StepperIndexer::sleep();

    // wake without maintaining shadows
    StepperIndexer::wake();
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
StepperIndexer::stepMicrostepWDelay(unsigned int milliseconds) {
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
    IndexerChipState::advanceMotorShadowState();
}


void
StepperIndexer::stepDetentWithDelay(unsigned int milliseconds) {
    /*
     * !!! Call DriverChipInterface.  Self controls speed.
     */
#if STEPPER_MICROSTEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
#elif STEPPER_MICROSTEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
    DriverChipInterface::stepMicrostep();
    Delay::inMilliseconds(milliseconds);
#elif STEPPER_MICROSTEP_SIZE_QUARTER
    StepperIndexer::stepMicrostepWDelay(milliseconds);
    StepperIndexer::stepMicrostepWDelay(milliseconds);
    StepperIndexer::stepMicrostepWDelay(milliseconds);
    StepperIndexer::stepMicrostepWDelay(milliseconds);
#else
    error
#endif
}


void
StepperIndexer::stepManyDetentsAtSpeed(
        unsigned int stepCount,
        MotorSpeed speed)
{
    for (unsigned int i = stepCount; i>0; i--) {
        stepDetentAtSpeed(speed);
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
#if STEPPER_MICROSTEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();

#elif STEPPER_MICROSTEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
#elif STEPPER_MICROSTEP_SIZE_QUARTER

    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
#else
    error
#endif
}


void
StepperIndexer::stepMicrostepMaxSpeed() {
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
}


void
StepperIndexer::stepDetentAtSpeed(MotorSpeed speed) {
    /*
     * This is to move motor, require coils enabled.
     */
    myAssert(IndexerChipState::isCoilsEnabled());

    // Appropriate count of microsteps each followed by appropriate delay.

#if STEPPER_MICROSTEP_SIZE_FULL
    // Each microstep is one full detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);

#elif STEPPER_MICROSTEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
#elif STEPPER_MICROSTEP_SIZE_QUARTER
    // Four microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
    DriverChipInterface::stepMicrostep();
    delayMicrostepForSpeed(speed);
#else
    error
#endif
}









/*
 * All delays are multiples of max delay,
 * since all speeds are fractions of max speed.
 */
void
StepperIndexer::delayMicrostepForSpeed(MotorSpeed speed) {
    switch(speed) {
    // Max calls another function
    case MotorSpeed::Max:
        delayMicrostepForMaxSpeed();
        break;

    // All other speeds recurse
    case MotorSpeed::Half:
        delayMicrostepForSpeed(MotorSpeed::Max);
        delayMicrostepForSpeed(MotorSpeed::Max);
        break;
    case MotorSpeed::Quarter:
        delayMicrostepForSpeed(MotorSpeed::Half);
        delayMicrostepForSpeed(MotorSpeed::Half);
        break;
    case MotorSpeed::Eighth:
        delayMicrostepForSpeed(MotorSpeed::Quarter);
        delayMicrostepForSpeed(MotorSpeed::Quarter);
        break;
    case MotorSpeed::Sixteenth:
        delayMicrostepForSpeed(MotorSpeed::Eighth);
        delayMicrostepForSpeed(MotorSpeed::Eighth);
        break;
    default:
        myAssert(false);
    }
}




void
StepperIndexer::delayMicrostepForMaxSpeed() {

    // For the defined motor max PPS


#if STEPPER_MICROSTEP_SIZE_FULL

    //
    // StepperIndexer::delayFor250PPS();
    // 100 PPS is 5 RPS, 300 RPM
#if MOTOR_MAX_PPS_FULL_STEP == 100
    StepperIndexer::delayFor100PPS();
#elif MOTOR_MAX_PPS_FULL_STEP == 600
    StepperIndexer::delayFor600PPS();
#else
    error
#endif

#elif STEPPER_MICROSTEP_SIZE_HALF

#if MOTOR_MAX_PPS_FULL_STEP == 100
    // For half stepping, pulse frequency is double
    StepperIndexer::delayFor200PPS();
#elif MOTOR_MAX_PPS_FULL_STEP == 600
    StepperIndexer::delayFor1_2kPPS();
#else
    error
#endif

#elif STEPPER_MICROSTEP_SIZE_QUARTER

#if MOTOR_MAX_PPS_FULL_STEP == 100
    // For quarter stepping, pulse frequency is quadruple
    StepperIndexer::delayFor400PPS();
#elif MOTOR_MAX_PPS_FULL_STEP == 600
    StepperIndexer::delayFor2_4kPPS();
#else
    error
#endif

#else
    error // microstep size not defined
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
    // TODO this is a hack
    // delayMicrostepForSpeed(MotorSpeed::Half);
    delayMicrostepForSpeed(MotorSpeed::Quarter);
}


// If not STEPPER_FAULT_DETECT_USED, this will have link errors if used
bool
StepperIndexer::isFault() {
    return DriverChipInterface::isFault();
}



