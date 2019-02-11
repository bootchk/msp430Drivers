
#include "stepperIndexer.h"

// DriverLib
#include <gpio.h>




/*
 * Test TI DRV8834 stepper motor driver
 *
 * Pulse the step pin repeatedly.
 *
 * Pins:
 * Step: P1.2
 * Dir:  P1.3
 * Not sleep: P1.4
 * M0: P1.5
 * Enable: P1.6
 *
 * Config pin of driver is high i.e. indexer mode
 */






namespace {

/*
 * If M0 is connected to P1.5 and P1.5 is low, mode is half
 */
StepMode stepMode = StepMode::Half;

// initially: DIR pin  low, direction is false
bool direction = false;




void delayOneMilliSecond() { __delay_cycles(1000); }
void delayOneMicroSecond() { __delay_cycles(1); }

/*
 * A required delay after any wake.
 * Ensure 1 milliSec before any subsequent step.
 */
void delayForWakeChange() {
    delayOneMilliSecond();
}

/*
 * Delay for any command (other than wake) till next step
 */
void delayForCommandChange() {
    delayOneMicroSecond();
}

}




/*
 * Depends on motor and stepping mode (full or micro-stepping)
 */
unsigned int StepperIndexer::stepsPerRev() {
    return MOTOR_STEPS_PER_REV * static_cast<int> (stepMode);
}



/*
 * Mode of driver (whether is microstepping)
 * Set on M0 and M1 pins
 *
 * M1 defaults to low and is not connected to mcu.
 *
 * M0 low => full step
 * M0 high => half step
 * M0 float => quarter step
 *
 * Requires current limit of driver set low enough (in hardware potentionmeter)
 * otherwise may skip microsteps
 */
void StepperIndexer::toFullStepMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Full;
}
void StepperIndexer::toHalfStepMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Half;
}
void StepperIndexer::toQuarterStepMode() {
    // Make pin float
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Quarter;
}





/*
 * Toggle NotSleep pin.
 */
void StepperIndexer::wake() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4);
    delayForWakeChange();
}
void StepperIndexer::sleep() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
}







void StepperIndexer::toggleDirection() {
    if (direction) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
        direction = false;
    }
    else {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3);
        direction = true;
    }
    delayForCommandChange();
}



/*
 * Pin is NotEnable: low is enabled, high is disabled.
 *
 * DriverChip has internal pulldown i.e. unconnected pin is in "enable" state
 */
void StepperIndexer::disableOutput() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}
void StepperIndexer::enableOutput() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}



void StepperIndexer::step() {
    /*
     * Pulse high the "step" pin.
     * If microstepping, not a full step.
     */
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // driver requires > 1.7uSec pulse high
    // assume procedure call overhead is enough
    //__delay_cycles(2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // driver requires > 1.7uSec pulse low
    //__delay_cycles(2);
}
