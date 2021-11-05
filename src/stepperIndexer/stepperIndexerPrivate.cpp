
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "stepperIndexer.h"

// Uses chip driver
#include "motor.h"

#include "src/delay/delay.h"
#include "src/assert/myAssert.h"





/*
 * For:
 * 20 step motor
 * half step mode
 *
 * 1 mSec delay is 1000 microsteps/sec, 500 detentstep/sec, 30k detent steps/min, 1500 rpm
 * 2 mSec delay is 500 microsteps/sec, 250 detentstep/sec, 15k detent steps/min, 750 rpm
 * 3 mSec delay is 333 microsteps/sec, 166 detentstep/sec, 10k detent steps/min, 500 rpm
 *
 * For:
 * 200 step motor e.g. SOYO NEMA
 * Full step
 *
 * At 200 or 300 uS,  skips steps.
 * At 400 uS,  works.  This might be the fastest it will spin.
 * At 1 mS, SOYO NEMA works, but slowly.
 */


/*
 * Delay per pulse, which may be a detent step or a microstep (depends on configuration of driver chip.)
 *
 * Max microstep frequency of DRV8834 is 250kHz i.e. 4uS per pulse
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
    // 4 mS
    delayFor500PPS();
    delayFor500PPS();
}

void
StepperIndexer::delayFor400PPS() {
    // 2.5 mS
    Delay::oneMillisecond();
    Delay::oneMillisecond();
    Delay::halfMillisecond();
}

void
StepperIndexer::delayFor500PPS() {
    // 2 mS
    Delay::oneMillisecond();
    Delay::oneMillisecond();
}

/*
 * 6k PPS is 166 uS/pulse
 * 4k PPS is 250 uS/pulse
 * 2.5k PPS is 400 uS/pulse
 */

// TODO so these are actually 2.5k full steps per second or 12.5 rev per second, or 720RPM
void
StepperIndexer::delayFor6000PPS() {
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
}

void
StepperIndexer::delayFor12000PPS() {
    Delay::hundredMicroseconds();
    Delay::hundredMicroseconds();
}

void
StepperIndexer::delayFor24000PPS() {
    Delay::hundredMicroseconds();
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




