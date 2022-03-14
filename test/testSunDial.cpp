
// DriverLib
#include <pmm.h>


#include "../src/pinFunction/allPins.h"
#include "../src/stepperMotor/stepperMotor.h"
#include "../src/stepperMotor/degreeStepperMotor.h"
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "../src/stepperIndexer/stepperIndexer.h"
#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"












static void
delayBetweenTests() {
    Delay::oneSecond();
    Delay::oneSecond();
}



/*
 * Expect: motor move one step (say 1/20 rev i.e. 18 degrees)
 */
static void
wakeStepSleep() {
   //StepperIndexer::sleep();

    StepperIndexer::wake();
    // assert wake restored driver to motor step

    DriverChipInterface::enableCoilDrive();
    myAssert(DriverChipInterface::isEnabledCoilDrive());

    // Step one detent with a fixed delay
    StepperIndexer::stepDetentWithDelay(100);

    StepperIndexer::sleep();
    DriverChipInterface::disableCoilDrive();
    myAssert( ! DriverChipInterface::isEnabledCoilDrive());


}



void
testSunDial() {

    //launchpadLEDOff();
    AllPins::setAllOutputsLow();
    AllPins::configureGPIOLowPower();

    PMM_unlockLPM5();

    StepperIndexer::wake();
    // assert in home state

    // Not runtime configurable for some boards
    // DriverChipInterface::toHalfStepSizeMode();

    DriverChipInterface::getStepSize();

    // StepperIndexer::syncDriverWithMotor();
    //StepperIndexer::findPhysicalStop(MotorDirection::Backward);

    DriverChipInterface::enableCoilDrive();

    // Test steps
    // Ensure holds at step when coils deenergized.
    // Repeat with a delay after every revolution (to insure we are not missing steps)


    while (true) {
        wakeStepSleep();

        //StepperIndexer::stepDetentWithDelay(100);
        // TODO deenergize

        delayBetweenTests();
    }




}
