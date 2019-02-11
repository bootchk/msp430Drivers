
// DriverLib
#include <pmm.h>
#include <gpio.h>



#include "stepperIndexer.h"









void launchpadLEDOff() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
}

void setAllOutputsLow() {
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;
}




 // configure all GPIO out to ensure low power
void configureGPIOLowPower() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3.
    // Datasheet says only 3 pins??
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
}





void delayOneSecond()      { __delay_cycles(1000000); }  // 1 second at 1Mhz clock
void delayTenthSecond()    { __delay_cycles(100000); }
void delayOneMilliSecond() { __delay_cycles(1000); }








void fullStep() {
    StepperIndexer::step();
    StepperIndexer::step();
    StepperIndexer::step();
    StepperIndexer::step();
}


/*
 * Full step without procedure call delays
 */
void fullStepSmooth()
{
GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);

GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);

GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);

GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
}


/*
 * Default config of breakout board is 1/4 step.
 * Motor is 200 steps.
 * 800 pulses per second would be rev per second, i.e. 60 rpm
 */
void step360() {
    for (unsigned int i = StepperIndexer::stepsPerRev(); i>0; i--) {
        StepperIndexer::step();
        delayOneMilliSecond();
    }
}

/*
 * Sleep between steps
 */
void step360Incrementally() {
    // 1/4 microstep
    for (unsigned int i = StepperIndexer::stepsPerRev(); i>0; i--) {
        // ??? wake and sleep make this not work
        //wake();
        StepperIndexer::step();
        //fullStep();
        //sleep();
        delayTenthSecond();
    }
}


void wakeFullStep() {
    StepperIndexer::wake();
    fullStepSmooth();
    StepperIndexer::sleep();
}

void step360IncrementallyFull()
{
    for (unsigned int i = StepperIndexer::stepsPerRev(); i > 0; i--)
    {
        // ??? wake and sleep make this not work
        //wake();
        fullStep();
        //sleep();
        delayTenthSecond();
    }
}





void testBackAndForth() {
    /*
     * Test repeatability by stepping back and forth one rev:
     * expect flag to stop at same place every time.
     */
    step360Incrementally();
    StepperIndexer::toggleDirection();
    StepperIndexer::sleep();
    delayOneSecond();
    StepperIndexer::wake();

    step360Incrementally();
    delayOneSecond();
    StepperIndexer::toggleDirection();
}


void testWakeStep() {
    /*
     * Forever step, then sleep.
     */
    while (true) {
        wakeFullStep();
    }
}



void testHomeState() {

    StepperIndexer::sleep();
    StepperIndexer::wake();
    // assert in home state (step 2 for half step)

    fullStepSmooth();
    fullStepSmooth();
    // assert motor in sync with step
    // assert in home state (step 2 for half step)

    // advance to state 3 where only one coil energized (detent state?)
    StepperIndexer::step();

    while (true)
    {
        StepperIndexer::sleep();
        StepperIndexer::wake();
        // assert in state 2

        // Quietly advance driver state to state 3
        StepperIndexer::disableOutput();
        StepperIndexer::step();
        StepperIndexer::enableOutput();

        // Advance another detent (8 steps)
        fullStepSmooth();
        fullStepSmooth();
        // assert in state 3
    }











}


void testStepperDriver() {

    //launchpadLEDOff();
    setAllOutputsLow();
    configureGPIOLowPower();

    PMM_unlockLPM5();

    StepperIndexer::wake();
    // assert in home state

    //StepperIndexer::toQuarterStepMode();
    StepperIndexer::toHalfStepMode();


    // Uncomment to test disabling
    //StepperIndexer::disableOutput();

    while(true) {
        testBackAndForth();

        // never returns
        //testWakeStep();
        testHomeState();
    }
}
