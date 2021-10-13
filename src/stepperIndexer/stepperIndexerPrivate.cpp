
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "stepperIndexer.h"

// Uses chip driver
#include "motor.h"

#include "src/delay/delay.h"














/*
 * Delay per microstep
 *
 * For:
 * 20 step motor
 * half step mode
 *
 * Max microstep frequency of DRV8834 is 250kHz
 * 1 mSec delay is 1000 microsteps/sec, 500 detentstep/sec, 30k detent steps/min, 1500 rpm
 * 2 mSec delay is 500 microsteps/sec, 250 detentstep/sec, 15k detent steps/min, 750 rpm
 * 3 mSec delay is 333 microsteps/sec, 166 detentstep/sec, 10k detent steps/min, 500 rpm
 */
void
StepperIndexer::delayAccordingToSpeed() {
    // TODO
    delayFor100PPS();
}

/*
 * Delay so motor turns at 100PPS (pulses per second).
 * For 20 step motor, that is 5 steps a second, or 300 RPM.
 *
 * PPS is specified for whole steps.
 * Delay appropriate for microsteps.
 */
void
StepperIndexer::delayFor100PPS() {
#if STEPPER_HARD_STEP_SIZE_FULL
    Delay::tenMilliseconds();
#elif STEPPER_HARD_STEP_SIZE_HALF
    Delay::fiveMilliseconds();
#endif
}

void
StepperIndexer::delayFor250PPS() {
    delayFor500PPS();
    delayFor500PPS();
}

void
StepperIndexer::delayFor500PPS() {
#if STEPPER_HARD_STEP_SIZE_FULL
    Delay::oneMillisecond();
    Delay::oneMillisecond();
#elif STEPPER_HARD_STEP_SIZE_HALF
    Delay::oneMillisecond();
#endif
}

void
StepperIndexer::delayFor6000PPS() {
#if STEPPER_HARD_STEP_SIZE_FULL
    // At 200 or 300 uS, SOYO NEMA skips steps.
    // At 400 uS, SOYO NEMA works.  This might be the fastest it will spin.
    // At 1 mS, SOYO NEMA works, but slowly.
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
#elif STEPPER_HARD_STEP_SIZE_HALF
    Delay::hundredMicroseconds();
#endif
}


void StepperIndexer::stepMicrostepAtSpeed() {

    // tell driver.  Driver has no explicit delays.
    DriverChipInterface::stepMicrostep();

    delayAccordingToSpeed();

    // We don't maintainShadowStep() here
}


void StepperIndexer::fastStepDetent() {
    // assert driver is disabled

    // tell driver, not self (no delay)
    DriverChipInterface::stepMicrostep();
    DriverChipInterface::stepMicrostep();
}




