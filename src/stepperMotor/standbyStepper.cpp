
#include "standbyStepper.h"


//#include "degreeStepperMotor.h"
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include <src/stepperIndexer/stepperIndexer.h>
#include <src/assert/myAssert.h>
#include <src/loadSwitch/loadSwitch.h>

/*
 * Expect: motor move one step (say 1/20 rev i.e. 18 degrees)
 * Ensure unpowered.
 */
void
StandbyStepperMotor::powerOnAndStepThenOff()
{
    /*
     * There is only one load switch and it is to the motor.
     * It is configured to output.
     */
    HighSideSwitch::turnOn();
    /*
     * Delay for driver chip after power on.
     */
    // TODO

    /*
     * Assert the driver is on the home step, which corresponds to the physical position of the motor.
     * I.E. enabling the coils will not move the rotor.
     */

    StepperIndexer::wake();
    // assert wake restored driver to motor step

    DriverChipInterface::enableCoilDrive();
    myAssert(DriverChipInterface::isEnabledCoilDrive());

    // Step one detent with a fixed delay
    StepperIndexer::stepDetentWithDelay(100);

    /*
     * Assert the delay is enough to kill momentum of rotor.
     */

    StepperIndexer::sleep();
    DriverChipInterface::disableCoilDrive();
    myAssert( ! DriverChipInterface::isEnabledCoilDrive());

    HighSideSwitch::turnOff();
}
