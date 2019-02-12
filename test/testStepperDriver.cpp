
// DriverLib
#include <pmm.h>
#include <gpio.h>



#include "../src/stepperIndexer/stepperIndexer.h"
#include "../src/stepperIndexer/driverChipInterface.h"









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








void fourStep() {
    // Four detent steps, i.e. 8 microsteps
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    StepperIndexer::stepDetent();
    // Assert returned to microstep and detent step on entry
}

#ifdef NOT_USED
/*
 * Full step without procedure call delays
 */
void fourStepSmooth()
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
#endif



/*
 * Default config of breakout board is 1/4 StepMode.
 * Motor is 200 steps.
 * 800 pulses per second would be rev per second, i.e. 60 rpm
 */
void step360() {
    for (unsigned int i = DriverChipInterface::stepsPerRev(); i>0; i--) {
        StepperIndexer::stepMicrostep();
        delayOneMilliSecond();
    }
}

/*
 * Sleep between steps
 */
void step360Incrementally() {
    // 1/4 microstep
    for (unsigned int i = DriverChipInterface::stepsPerRev(); i>0; i--) {
        // ??? wake and sleep make this not work
        //wake();
        StepperIndexer::stepMicrostep();
        //fourStep();
        //sleep();
        delayTenthSecond();
    }
}


#ifdef NOT_USED
void wakefourStep() {
    StepperIndexer::wake();
    fourStepSmooth();
    StepperIndexer::sleep();
}
#endif


void step360IncrementallyFull()
{
    for (unsigned int i = DriverChipInterface::stepsPerRev(); i > 0; i--)
    {
        // ??? wake and sleep make this not work
        //wake();
        fourStep();
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
    DriverChipInterface::toggleDirection();
    StepperIndexer::sleep();
    delayOneSecond();
    StepperIndexer::wake();

    step360Incrementally();
    delayOneSecond();
    DriverChipInterface::toggleDirection();
}


#ifdef NOT_USED
void testWakeStep() {
    /*
     * Forever step, then sleep.
     */
    while (true) {
        wakefourStep();
    }
}
#endif


void testHomeState() {

    // assert DriverChip just powered up and is awake (NotSleep pin defaults to low)
    StepperIndexer::sleep();
    StepperIndexer::wake();
    // assert DriverChip in home state (microstep 2 for Half StepMode)
    // assert motor is on unknown step, but stepOfMotor is also HomeStep == 2

    /*
     * Sync motor with DriverChip
     */
    fourStep();
    fourStep();
    // assert motor in sync with microstep
    // assert in home state (microstep 2 for Half StepMode)
    // assert microstep is a DetentStep

    StepperIndexer::setShadowStepOfDriver(2);

    // advance to state 3 where only one coil energized (detent state?)
    //StepperIndexer::microstep();

    while (true)
    {
        StepperIndexer::sleep();

        StepperIndexer::wake();
        // assert wake restored driver to motor step

        // WAS StepperIndexer::restoreDriverToMotorStep();
        // Quietly advance driver state to current shadow step

        // Step one detent
        StepperIndexer::stepDetent();

        // Advance another detent (8 steps)
        //fourStep();
        //fourStep();
        // assert in state 3

        delayTenthSecond();
        delayTenthSecond();
        delayTenthSecond();

        // assert shadow state advanced by microsteps per detent step
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
    DriverChipInterface::toHalfStepMode();


    // Uncomment to test disabling
    //StepperIndexer::disableOutput();

    while(true) {
        testBackAndForth();

        // never returns
        //testWakeStep();
        testHomeState();
    }
}
