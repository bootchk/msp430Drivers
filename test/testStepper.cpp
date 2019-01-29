
#include "../src/stepper/stepper.h"
// TODO stepper should have stepperDriver API and delegate
#include "../src/stepper/stepperDriver.h"


// DriverLib
#include <pmm.h>


#define StepsPerTest 25

void manySingleStepForward() {
    for (int i = StepsPerTest; i > 0; i--)
    {
        Stepper::singleStepForward();
        __delay_cycles(500000);
    }
}

void testStepper()
{
    Stepper::reset();
    // assert GPIO configured

    PMM_unlockLPM5();

    StepperDriver::turnPowerOn();

    // defaults to braking, low torque

    // braking, low torque
    manySingleStepForward();

    return;

    // coasting, low torque
    Stepper::setCoastingMode(false);
    manySingleStepForward();


    // single step forward, braking, low torque
    Stepper::setHighTorqueMode(true);
    manySingleStepForward();

    Stepper::setCoastingMode(true);
    Stepper::setHighTorqueMode(true);

}
