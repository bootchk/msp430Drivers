
#include "standbyStepper.h"


#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include <src/stepperIndexer/stepperIndexer.h>
#include <src/assert/myAssert.h>
#include <src/loadSwitch/loadSwitch.h>
#include <src/delay/delay.h>




void
StandbyStepperMotor::powerOn()
{
    /*
    * Configure mode,
    * Mode is latched at power up and when cobing out of sleep.
    * Failed to work when hardwired pin Config to pin Vint.
    */
   DriverChipInterface::configureIndexerMode();

   /*
    * There is only one load switch and it is to the motor.
    * It is configured to output.
    */
   HighSideSwitch::turnOn();
   /*
    * Delay for driver chip after power on.
    *
    * Experimentally determined.
    */
   Delay::oneMillisecond();
}





/*
 * Expect: motor move one step (say 1/20 rev i.e. 18 degrees)
 * Ensure unpowered.
 */
void
StandbyStepperMotor::powerOnAndStepThenOff()
{
    StandbyStepperMotor::powerOn();

    /*
     * Assert the driver is on the home step,
     * and we powered off at the home step.
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
    DriverChipInterface::unconfigureIndexerMode();
}
