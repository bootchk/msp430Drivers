
#include "chipInterface.h"

#include "motor.h"

#include "../../assert/myAssert.h"
#include "../../delay/delay.h"





namespace {

void delayForDirectionChangeInertia() {
    /*
     * This is experimentally determined,
     * and depends on the motor and other parts of the system.
     * 1 mS does not seem to be enough (results in motor skips)
     * 10 mS seems to be enough for Symbol Tech 21-02485
     */
    // Delay::oneMillisecond();
    Delay::tenMilliseconds();
}

} // namespace




/*
 * Depends on motor and stepping mode (full or micro-stepping)
 */
unsigned int DriverChipInterface::microstepsPerRev() {
    return MOTOR_STEPS_PER_REV * IndexerChipState::microStepsPerDetentStep();
}

unsigned int DriverChipInterface::detentstepsPerRev() {
    return MOTOR_STEPS_PER_REV;
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

#if STEPPER_MICROSTEP_SIZE_FULL
    return StepSizeMode::Full;
#elif STEPPER_MICROSTEP_SIZE_HALF
    return StepSizeMode::Half;
#elif STEPPER_MICROSTEP_SIZE_QUARTER
    return StepSizeMode::Quarter;
#else
error
#endif
}



void DriverChipInterface::setDirectionAndRelease(MotorDirection direction) {
    /*
     * Disable coils in anticipation of next step in opposite direction.
     * !!! If the next step is not soon, holding torque is reduced.
     */
    disableCoilDrive();

    /*
     * Tell the chip.
     */
    switch(direction) {
    case MotorDirection::Forward:
        GPIO_setOutputHighOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    case MotorDirection::Backward:
        GPIO_setOutputLowOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    default:
        myAssert(false);
    }

    delayForDirectionChangeInertia();

    IndexerChipState::setDirection(direction);
}


void DriverChipInterface::setDirectionAndHold(MotorDirection direction) {
    /*
     * Not disable coils in anticipation of next step in opposite direction.
     * If motor has inertia, next step might be ineffective
     */

    switch(direction) {
    case MotorDirection::Forward:
        GPIO_setOutputHighOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    case MotorDirection::Backward:
        GPIO_setOutputLowOnPin(STEPPER_DIR_PORT, STEPPER_DIR_PIN);
        break;
    default:
        myAssert(false);
    }

    /*
     * Delay for command but not for motor inertia.
     */
    delayForCommand();

    // Remember SW state
    IndexerChipState::setDirection(direction);
}







//#pragma FUNC_ALWAYS_INLINE
//inline
void
DriverChipInterface::stepMicrostep() {

    /*
     * Not require coils enabled.
     * This may be called to simply step the chip in its internal table,
     * without moving motor.
     */

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


#if STEPPER_FAULT_DETECT_USED
/*
 * The notFault pin resets itself when the condition clears.
 * So polling it is not sufficient (unless the condition is long duration.)
 * This should be an interrupt.
 */
bool
DriverChipInterface::isFault() {
    GPIO_setAsInputPin(STEPPER_FAULT_PORT, STEPPER_FAULT_PIN);
    // pin is notFault
    return ! GPIO_getInputPinValue(STEPPER_FAULT_PORT, STEPPER_FAULT_PIN);
}

void
DriverChipInterface::enableInterruptOnFault() {

    // Require enable general interrupts

    // Enable interrupt on fault pin
    GPIO_clearInterrupt     (STEPPER_FAULT_PORT, STEPPER_FAULT_PIN);
    GPIO_setAsInputPin      (STEPPER_FAULT_PORT, STEPPER_FAULT_PIN);
    GPIO_selectInterruptEdge(STEPPER_FAULT_PORT, STEPPER_FAULT_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt    (STEPPER_FAULT_PORT, STEPPER_FAULT_PIN);
}

#endif // STEPPER_FAULT_DETECT_USED
