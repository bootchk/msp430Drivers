
#include "driverChipInterface.h"

// DriverLib
#include <gpio.h>

#include "motor.h"










namespace {

/*
 * When:
 * - M0 is connected to P1.5 and P1.5 is low
 * - M1 is unconnected and internally pulled down
 * => mode is half
 */
/*
 * Not persistent.
 * Currently code assumes Half (except stepsPerRev references it)
 * In many cases, the pins will be hardwired to a certain stepMode.
 */
#if STEPPER_HARD_STEP_SIZE_FULL
    StepSizeMode stepMode = StepSizeMode::Full;
#endif

/*
 * initially: DIR pin  low, direction is forward???
 */
/*
 * Not persistent.
 * A sleep-then-wake will not alter the direction.
 * A LPM4.5 sleep of mcu would probably alter the pins controlling direction,
 * but I assume you will set direction explicitly before any stepping.
 */
MotorDirection _direction = MotorDirection::Forward;




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
unsigned int DriverChipInterface::microstepsPerRev() {
    return MOTOR_STEPS_PER_REV * static_cast<int> (stepMode);
}

unsigned int DriverChipInterface::detentstepsPerRev() {
    return MOTOR_STEPS_PER_REV;
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





#if STEPPER_STEP_SIZE_RUNTIME_CHOOSEABLE

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
void DriverChipInterface::toFullStepSizeMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepSizeMode::Full;
}
void DriverChipInterface::toHalfStepSizeMode() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepSizeMode::Half;
}
void DriverChipInterface::toQuarterStepSizeMode() {
    // Make pin float
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN5);
    delayForCommandChange();
    stepMode = StepSizeMode::Quarter;
}

#endif


StepSizeMode
DriverChipInterface::getStepSize() {

#if STEPPER_HARD_STEP_SIZE_FULL
    return StepSizeMode::Full;
#else
#warning
#endif
}



void DriverChipInterface::setDirection(MotorDirection direction) {
    switch(direction) {
    case MotorDirection::Forward:
        GPIO_setOutputHighOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    case MotorDirection::Backward:
        GPIO_setOutputLowOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    }
    delayForCommandChange();
    _direction = direction;
}


MotorDirection DriverChipInterface::getDirection() { return _direction; }





/*
 * Pin is NotEnable: low is enabled, high is disabled.
 *
 * DriverChip has internal pulldown i.e. unconnected pin is in "enable" state
 */
void DriverChipInterface::disableCoilDrive() {
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}
void DriverChipInterface::enableCoilDrive() {
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
    delayForCommandChange();
}



/*
 * Pulse high the "step" pin.
 * A microstep might not be a full step.
 */
//#pragma FUNC_ALWAYS_INLINE
//inline
void
DriverChipInterface::stepMicrostep() {
    /*
     * Chip spec requires > 1.7uSec pulse high and low.
     * We assume procedure call overhead is enough
     * i.e. cpu speed is about 1Mhz and procedure call requires greater than two uSec.
     */
    GPIO_setOutputHighOnPin(STEPPER_STEP_PORT, STEPPER_STEP_PIN);
    // assert 2 uSec have passed
    GPIO_setOutputLowOnPin (STEPPER_STEP_PORT, STEPPER_STEP_PIN);
    // assert 2 uSec have passed
}
