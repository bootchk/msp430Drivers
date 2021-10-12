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
}


void StepperIndexer::initialWake() {
    DriverChipInterface::wake();
}


void StepperIndexer::wake() {
    // GPIO for enable coil drive may still be ON and coils driven?

    DriverChipInterface::wake();
}
