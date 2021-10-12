
#include "chipState.h"

#include "chipInterface.h"

#include "motor.h"


namespace {

/*
 * When:
 * - M0 is connected to low
 * - M1 is unconnected and internally pulled down
 * => step mode is full
 */
/*
 * Not persistent.
 * Currently code assumes Half (except stepsPerRev references it)
 * In many cases, the pins will be hardwired to a certain stepMode.
 */
#if STEPPER_HARD_STEP_SIZE_FULL
    StepSizeMode stepMode = StepSizeMode::Full;
#elif STEPPER_HARD_STEP_SIZE_HALF
    StepSizeMode stepMode = StepSizeMode::Half;
#else
    error
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


bool _isCoilsEnabled = false;

unsigned int  _microstepState = 2;


} // namespace




MotorDirection
IndexerChipState::getDirection() { return _direction; }

void
IndexerChipState::setDirection(MotorDirection direction) { _direction = direction; }




unsigned int
IndexerChipState::microStepsPerDetentStep() {
    return static_cast<int> (stepMode);
}



/*
 * Coil output enabled.
 */
bool
IndexerChipState::isCoilsEnabled() {
    return _isCoilsEnabled;
}

void
IndexerChipState::enableCoils(bool isEnabled) {
    _isCoilsEnabled = isEnabled;
}



/*
 * Step state.
 *
 * Steps are numbered.
 * After wake, the chip is in a certain numbered step e.g. 2
 */

void
IndexerChipState::rememberMotorStep(){
    // TODO shadowMicrostepOfMotor = shadowMicrostepOfDriver;
}


void
IndexerChipState::setMicrostepState(unsigned int aStep) {
    _microstepState = aStep;
}



/*
 * TODO
// quietly => non-energized
void
IndexerChipState::restoreDriverToMotorStep() {
    // assert motor is on a detentStep
    // assert chip is wake, in fact just waked from sleep and reset

    // 2 is the state after reset when half step mode
    // TODO get the step number by step mode
    setMicrostepState(2);

    // Prevent motor movement
    // TODO remember the actual state and restore it
    DriverChipInterface::disableCoilDrive();

    while ( not (shadowMicrostepOfDriver == shadowMicrostepOfMotor) ) {
        // Here we don't need a delay for speed, since motor is not turning, but we do need update shadow.
        // TODO fastStepDetent();
    }

    DriverChipInterface::enableCoilDrive();
}
*/
