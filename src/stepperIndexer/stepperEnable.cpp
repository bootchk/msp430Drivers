/*
 * Part of class StepperIndexer
 *
 * The part about sleep and enable outputs.
 */

#include "stepperIndexer.h"

// Uses chip driver
#include <src/stepperIndexer/chipInterface/chipInterface.h>


void
StepperIndexer::sleep() {

    DriverChipInterface::disableCoilDrive();
    DriverChipInterface::sleep();

    rememberMotorStep();
}


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
