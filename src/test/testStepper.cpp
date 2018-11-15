
#include "../stepper/stepper.h"

// DriverLib
#include <pmm.h>


#define StepsPerTest 25

void singleStepForward() {
    for (int i = StepsPerTest; i > 0; i--)
    {
        Stepper::singleStepForward();
        __delay_cycles(500000);
    }
}

void testStepper()
{
    Stepper
    ::reset();
    // assert GPIO configured

    PMM_unlockLPM5();

    Stepper::turnPowerOn();

    // defaults to braking, low torque

    // braking, low torque
    singleStepForward();

    return;

    // coasting, low torque
    Stepper::setCoastingMode(false);
    singleStepForward();


    // single step forward, braking, low torque
    Stepper::setHighTorqueMode(true);
    singleStepForward();

    Stepper::setCoastingMode(true);
    Stepper::setHighTorqueMode(true);

}
