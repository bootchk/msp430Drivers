
#include "chipState.h"

#include "chipInterface.h"

#include "motor.h"

#include <src/assert/myAssert.h>



// Include back up the tree, but not circular?
#include "../stepperIndexer.h"


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
#if STEPPER_MICROSTEP_SIZE_FULL
    StepSizeMode stepMode = StepSizeMode::Full;
#elif STEPPER_MICROSTEP_SIZE_HALF
    StepSizeMode stepMode = StepSizeMode::Half;
#elif STEPPER_MICROSTEP_SIZE_QUARTER
    StepSizeMode stepMode = StepSizeMode::Quarter;
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


/*
 * Step of the DriverChip.
 * The DriverChip will not tell us what step it is on.
 * We shadow it, i.e. keep track algorithmically.
 *
 * Not persistent and no initial value.
 * wake() sets the initial value, advancing it to match the motor.
 */
unsigned int shadowMicrostepOfDriver;


// TODO motor state is not part of chip state, but belongs to the high level API
/*
 * Step the motor is on.
 *
 * We don't know this until we sync the motor with the DriverChip.
 * Then we assume the motor stays in sync (we don't keep it updated while stepping motor.)
 * We remember this when we sleep.
 * When we wake, resetting the DriverChip resets its internal state to its HomeStep,
 * and we advance DriverChip state and shadowMicrostepOfDriver to match the remembered shadowMicrostepOfMotor.
 */
/*
 * Persistent.
 * Even if you unpower the mcu, we remember the step of the motor.
 *
 * Initialized to an arbitrary cogging (natural detent) step.
 * TODO ???? is this right.
 * 1 is zero electrical angle.
 */

#pragma PERSISTENT
unsigned int shadowMicrostepOfMotor = STEPPER_COGGING_STEP;
}


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
    myAssert(false);
}


void
IndexerChipState::setMicrostepState(unsigned int aStep) {
    shadowMicrostepOfDriver = aStep;
}


void
IndexerChipState::setDriverShadowStateToHomeStep()
{
    /* See motor.h */
    shadowMicrostepOfDriver = HOME_MICROSTEP;
}


// TODO this is for quarter stepping only
// TODO this is for forward only
void
IndexerChipState::advanceDriverShadowState()
{
    shadowMicrostepOfDriver += 1;
    // Rollover, using 1-based counting, same as documentation
    if (shadowMicrostepOfDriver > COUNT_MICROSTEP_STATES)
        shadowMicrostepOfDriver = 1;
    // assert step state is in [1,COUNT_MICROSTEP_STATES]
}


// TODO this is for quarter stepping only
void
IndexerChipState::advanceMotorShadowState()
{
    shadowMicrostepOfMotor += 1;
    // Rollover, using 1-based counting, same as documentation
    if (shadowMicrostepOfMotor > COUNT_MICROSTEP_STATES)
        shadowMicrostepOfMotor = 1;
}



// quietly => non-energized
void
IndexerChipState::restoreDriverToMotorStep() {
    /*
     * assert motor is on a detentStep
     * assert chip is just waked from sleep and step is home step.
     */


    /*
     * Prevent motor movement while stepping the chip itself.
     */
    DriverChipInterface::disableCoilDrive();

    while ( not (shadowMicrostepOfDriver == shadowMicrostepOfMotor) ) {
        /*
         * Here we don't need a delay for speed, since motor is not turning, but we do need update shadow.
         */
        StepperIndexer::fastStepMicrostep();
        // assert the chip's state, and the SW shadow step state, were advanced.
    }

    // We don't ensure the coils are re enabled.
    // DriverChipInterface::enableCoilDrive();
}



/*
 * Shadow the driver's microstep index.
 *
 * Modulo 8, for half step microstepping
 */
// TODO other step modes
/*
void StepperIndexer::maintainShadowStep() {
    // assert we just microstepped twice (one detentStep)
    switch(IndexerChipState::getDirection()) {
    case MotorDirection::Forward:
    //case MotorDirection::Backward:
        shadowMicrostepOfDriver += 2;
        if (shadowMicrostepOfDriver > 7)
                shadowMicrostepOfDriver = 0;
        break;
    case MotorDirection::Backward:
    //case MotorDirection::Forward:

        if (shadowMicrostepOfDriver == 0)
                shadowMicrostepOfDriver = 8;
        shadowMicrostepOfDriver -= 2;
        break;
    }
    // WAS assert shadowMicrostepOfDriver in [0..7]
    // assert shadowMicrostepOfDriver in [0, 2, 4, 6]
}
*/
