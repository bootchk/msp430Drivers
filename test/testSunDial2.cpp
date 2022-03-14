
// DriverLib
#include <pmm.h>

#include "../src/pinFunction/allPins.h"
#include "../src/stepperMotor/stepperMotor.h"
#include "../src/stepperMotor/degreeStepperMotor.h"
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "../src/stepperIndexer/stepperIndexer.h"
#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"
#include "../src/stepperMotor/standbyStepper.h"
#include "../src/timer/timer.h"






bool
isPowerForStepping()
{
    // TODO
    return true;
}


void
delayInLowPowerMode()
{
    //  not low power
    //Delay::oneSecond();

    LowPowerTimer::delaySeconds(1);
}



/*
 * Test stepping a motor,
 * where the motor driver is unpowered between steps,
 * and enter low power mode.
 */
void
testSunDial2() {

    AllPins::setAllOutputsLow();
    AllPins::configureGPIOLowPower();

    PMM_unlockLPM5();

    /*
     * Assert the high side load switch for the stepper driver is configured and off.
     */

    while (true) {
        if (isPowerForStepping())
        {
            StandbyStepperMotor::powerOnAndStepThenOff();
        }

        /*
         * For testing purposes, we don't need to be in low power mode sleep.
         * But it will drain the storage supercap sooner.
         */
        delayInLowPowerMode();
    }

}
