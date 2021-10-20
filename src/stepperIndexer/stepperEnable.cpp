/*
 * Part of class StepperIndexer
 *
 * The part about sleep and enable outputs.
 */

#include "stepperIndexer.h"

// Uses chip driver
#include <src/stepperIndexer/chipInterface/chipInterface.h>

#if STEPPER_SLEEP_USED
void
StepperIndexer::sleep() {

    DriverChipInterface::disableCoilDrive();
    DriverChipInterface::sleep();
}


void StepperIndexer::wake() {
    // GPIO for enable coil drive may still be ON and coils driven?

    DriverChipInterface::wake();
}

#else
    // chip is always enabled by HW and these are no ops
void StepperIndexer::sleep() {}
void StepperIndexer::wake() {}
#endif
