
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




void StepperIndexer::findPhysicalStop(MotorDirection direction) {


    DriverChipInterface::wake();
    DriverChipInterface::setDirection(direction);
    DriverChipInterface::enableCoilDrive();

    /*
     * Turn one revolution (enough to encounter the physical stop.)
     */
    stepManyDetentsAtSpeed(MOTOR_STEPS_PER_REV, MotorSpeed::Quarter);

    DriverChipInterface::disableCoilDrive();

    // reset driver chip
    DriverChipInterface::sleep();
    DriverChipInterface::wake();

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
#else
    error
#endif
}


void
StepperIndexer::stepManyDetentsAtSpeed(
        unsigned int stepCount,
        MotorSpeed speed) {
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
    delayMicrostepForMaxSpeed());

#elif STEPPER_MICROSTEP_SIZE_HALF
    // Two microstep per detentstep
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
    DriverChipInterface::stepMicrostep();
    delayMicrostepForMaxSpeed();
#else
    error
#endif
}


void
StepperIndexer::stepDetentAtSpeed(MotorSpeed speed) {
    /*
     * This is to move motor, require coils enabled.
     */
    myAssert(IndexerChipState::isCoilsEnabled());

    /*
     * Do appropriate count of microsteps each followed by appropriate delay.
     */

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
#else
    error
#endif
    // TODO temp test
    if (StepperIndexer::isFault())
        myAssert(false);
}









/*
 * All delays are multiples of max delay,
 * since all speeds are fractions of max speed.
 */
void
StepperIndexer::delayMicrostepForSpeed(MotorSpeed speed) {
    switch(speed) {
    case MotorSpeed::Max:
        delayMicrostepForMaxSpeed();
        break;
    case MotorSpeed::Half:
        delayMicrostepForMaxSpeed();
        delayMicrostepForMaxSpeed();
        break;
    case MotorSpeed::Quarter:
        delayMicrostepForMaxSpeed();
        delayMicrostepForMaxSpeed();
        delayMicrostepForMaxSpeed();
        delayMicrostepForMaxSpeed();
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
#if MOTOR_MAX_PPS == 100
    StepperIndexer::delayFor100PPS();
#else
    error
#endif

#elif STEPPER_MICROSTEP_SIZE_HALF

#if MOTOR_MAX_PPS == 100
    // For half stepping, pulse frequency is double
    StepperIndexer::delayFor200PPS();

    // PPS of 100 halves the max speed
    // StepperIndexer::delayFor100PPS();
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


bool
StepperIndexer::isFault() {
    return DriverChipInterface::isFault();
}


