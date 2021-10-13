
// DriverLib
#include <pmm.h>
#include <gpio.h>

#include "../src/stepperIndexer/stepperMotor.h"
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "../src/stepperIndexer/stepperIndexer.h"
#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"









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









/*
 */
void step360Jerky() {
    for (unsigned int i = DriverChipInterface::detentstepsPerRev(); i>0; i--) {
        StepperIndexer::stepDetentMaxSpeed();
        // Delays between microsteps appropriate to speed
        // This may still be jerky and slow because of loop overhead.
    }
}


// Turn one rev as fast as SW and HW allow
void step360Smooth() {

    unsigned int stepsPerRev = DriverChipInterface::detentstepsPerRev();
    StepperIndexer::stepManyDetents(stepsPerRev);
}


/*
 * Delay between steps, i.e. slowly.
 * Here we do our own delays, instead of delays in the called routine.
 */
void step360Slowly() {
    for (unsigned int i = DriverChipInterface::detentstepsPerRev(); i>0; i--) {

        // Delay after each detent step, so it is jerky
        StepperIndexer::stepDetentWithDelay(100);


        // Delay::hundredMilliseconds();
    }
}

void turnQuarterRev(MotorDirection direction) {
    // 5 steps @18degrees is 90 degrees
    StepperMotor::wakeTurnAndSleep(5, direction);
}



void
sleepAndDelayBetweenTests() {
    StepperIndexer::sleep();
    Delay::oneSecond();
    Delay::oneSecond();
    StepperIndexer::wake();
    // Motor should not move on wake
}

void
delayBetweenTests() {
    Delay::oneSecond();
    Delay::oneSecond();
}



void
testPecking() {
    while (true) {
        // Forward to some contact
        turnQuarterRev(MotorDirection::Forward);

        // Peck
        StepperMotor::wakeTurnAndSleep(1, MotorDirection::Backward);
        StepperMotor::wakeTurnAndSleep(1, MotorDirection::Forward);

        // assert is sleep
        turnQuarterRev(MotorDirection::Backward);

        Delay::oneSecond();
    }
}


void
testQuarterRevs() {
    while (true) {
        turnQuarterRev(MotorDirection::Forward);

        // assert is sleep
        turnQuarterRev(MotorDirection::Backward);

        Delay::oneSecond();
    }
}



void testBackAndForth() {
    /*
     * Test repeatability by stepping back and forth one rev:
     * expect flag to stop at same place every time.
     */

    delayBetweenTests();
    DriverChipInterface::setDirection(MotorDirection::Forward);
    step360Jerky();
    delayBetweenTests();
    DriverChipInterface::setDirection(MotorDirection::Backward);
    step360Jerky();

    delayBetweenTests();
    DriverChipInterface::setDirection(MotorDirection::Backward);
    //step360Smooth();

    delayBetweenTests();
    DriverChipInterface::setDirection(MotorDirection::Forward);
    step360Slowly();
    DriverChipInterface::setDirection(MotorDirection::Backward);
    step360Slowly();
}




/*
 * Expect: motor move one step (say 1/20 rev i.e. 18 degrees)
 */
void wakeStepSleep() {
           //StepperIndexer::sleep();

            StepperIndexer::wake();
            // assert wake restored driver to motor step

            // Step one detent with a fixed delay
            StepperIndexer::stepDetentWithDelay(100);

            StepperIndexer::sleep();

            Delay::hundredMilliseconds();
            Delay::hundredMilliseconds();
}



/*
 * Expect motor twitch back and forth one step i.e. 18 degrees.
 *
 * For about 20 reps
 */
void
testHomeState() {

    // assert DriverChip just powered up and is awake (NotSleep pin defaults to low)

    StepperIndexer::syncDriverWithMotor();
    // assert is wake

    for (unsigned int i = 20; i>0; i--) {
        DriverChipInterface::setDirection(MotorDirection::Forward);
        wakeStepSleep();
        DriverChipInterface::setDirection(MotorDirection::Backward);
        // undo forward
        wakeStepSleep();
    }
}


void
testStepperDriver() {

    //launchpadLEDOff();
    setAllOutputsLow();
    configureGPIOLowPower();

    PMM_unlockLPM5();

    StepperIndexer::initialWake();
    // assert in home state

    // Not runtime configurable for some boards
    // DriverChipInterface::toHalfStepSizeMode();

    // assert is FullStepSize
    DriverChipInterface::getStepSize();

    StepperIndexer::syncDriverWithMotor();


    // Uncomment to test disabling
    //StepperIndexer::disableOutput();

    while(true) {

        delayBetweenTests();

        // does not return
        testPecking();

        // does not return
        testQuarterRevs();

        testBackAndForth();

        delayBetweenTests();

        //testHomeState();

        delayBetweenTests();

        // TODO stepQuarterRev();
        //testWakeStep();
    }
}
