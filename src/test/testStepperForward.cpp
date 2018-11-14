
#include "../stepper/stepper.h"




void testStepperForward()
{
    Stepper
    ::reset();
    // assert GPIO configured

    // TODO unlock lpm5

    Stepper::turnPowerOn();

    // forever, single step forward
    while (true)
    {
        Stepper
        ::singleStepForward();

        __delay_cycles(500000);
    }
}
