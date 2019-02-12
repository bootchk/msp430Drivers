
#include "driverChipInterface.h"

// DriverLib
#include <gpio.h>




/*
 * Connections:
 * MSP430 to TI DRV8834 stepper motor driver
 *
 * Pins:
 * Step: P1.2
 * Dir:  P1.3
 * Not sleep: P1.4
 * M0: P1.5
 * Enable: P1.6
 *
 * Config pin of driver chip is high i.e. indexer mode
 */




// depends on motor
//#define STEPS_PER_REV 200
#define MOTOR_STEPS_PER_REV 20


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
 * Chip spec requires delay after any wake before chip is active.
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
unsigned int DriverChipInterface::stepsPerRev() {
    return MOTOR_STEPS_PER_REV * static_cast<int> (stepMode);
}





/*
 * Toggle NotSleep pin.
 *
 * Low-level, not concerned with shadowing.
 */
void DriverChipInterface::wake() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4);

    delayForWakeChange();

    // assert DriverChip in state 2
    // assert motor is at remembered motor step, not necessarily 2
}

void DriverChipInterface::sleep() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
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
void DriverChipInterface::toFullStepMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Full;
}
void DriverChipInterface::toHalfStepMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Half;
}
void DriverChipInterface::toQuarterStepMode() {
    // Make pin float
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepMode::Quarter;
}





void DriverChipInterface::toggleDirection() {
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
void DriverChipInterface::disableOutput() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}
void DriverChipInterface::enableOutput() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}




void DriverChipInterface::stepMicrostep() {
    /*
     * Pulse high the "step" pin.
     * If microstepping, not a full step.
     */
    /*
     * Chip spec requires > 1.7uSec pulse high and low.
     * We assume procedure call overhead is enough
     * i.e. cpu speed is about 1Mhz and procedure call requires greater than two uSec.
     */
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // assert 2 uSec have passed
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // assert 2 uSec have passed
}
