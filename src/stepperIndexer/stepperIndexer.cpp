
/*
 * Implementation of public methods
 */


#include "stepperIndexer.h"

// Uses chip driver
#include "driverChipInterface.h"



void StepperIndexer::syncDriverWithMotor() {
    // reset driver chip
    DriverChipInterface::sleep();

    // wake without maintaining shadows
    DriverChipInterface::wake();
    // assert DriverChip in home state (microstep 2 for Half StepMode)
    // assert motor is on unknown step, but shadowStepOfMotor is also HomeStep == 2

    // enough steps for motor to catch.  Two complete cycles through whole steps
    // This is for a four whole step motor
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();

    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();

    // assert motor in sync with microstep
    // assert driver chip in home state (microstep 2 for Half StepMode)
    // assert microstep is a DetentStep

    StepperIndexer::setShadowMicrostepOfDriver(2);
}






// DriverChip is in Half step mode.
// TODO allow other step modes
void StepperIndexer::stepDetent() {
    // !!! Call to self, not to DriverChipInterface.  Self controls speed.
    stepMicrostep();
    stepMicrostep();
    maintainShadowStep();
}



/*
 * sleep/wake
 */

void StepperIndexer::initialWake() {
    DriverChipInterface::wake();
}


void StepperIndexer::wake() {
    // assert coil drive is disabled
    DriverChipInterface::wake();
    // assert driver chip is wake and reset
    // !!! but not enable coil drive

    // assert DriverChip in state 2
    // assert motor is at remembered motor step

    restoreDriverToMotorStep();
    // assert DriverChip is at same step as motor
    // assert coil drive enabled
}


void StepperIndexer::sleep() {

    DriverChipInterface::disableCoilDrive();
    DriverChipInterface::sleep();

    rememberMotorStep();
}

