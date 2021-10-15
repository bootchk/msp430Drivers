
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "stepperIndexer.h"

// Uses chip driver
#include "motor.h"

#include "src/delay/delay.h"
#include "src/assert/myAssert.h"






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
    myAssert(false); // Not implemented
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
    Delay::tenMilliseconds();
}

void
StepperIndexer::delayFor200PPS() {
    // 5 mS
    Delay::oneMillisecond();
    Delay::oneMillisecond();
    Delay::oneMillisecond();
    Delay::oneMillisecond();
    Delay::oneMillisecond();
}

void
StepperIndexer::delayFor250PPS() {
    delayFor500PPS();
    delayFor500PPS();
}

void
StepperIndexer::delayFor500PPS() {
    Delay::oneMillisecond();
    Delay::oneMillisecond();
}

void
StepperIndexer::delayFor6000PPS() {
#if STEPPER_MICROSTEP_SIZE_FULL
    // At 200 or 300 uS, SOYO NEMA skips steps.
    // At 400 uS, SOYO NEMA works.  This might be the fastest it will spin.
    // At 1 mS, SOYO NEMA works, but slowly.
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
#elif STEPPER_MICROSTEP_SIZE_HALF
    Delay::hundredMicroseconds();
#else
    error
#endif
}


void StepperIndexer::stepMicrostepAtSpeed() {

    // tell driver.  Driver has no explicit delays.
    DriverChipInterface::stepMicrostep();

    delayAccordingToSpeed();
}



void StepperIndexer::fastStepDetent() {
    myAssert(IndexerChipState::isCoilsEnabled() == false);

    // TODO why only two microsteps??
    // tell driver, not self (no delay)
    DriverChipInterface::stepMicrostep();
    DriverChipInterface::stepMicrostep();
}




