
#include "stepperIndexer.h"

// Uses chip driver
#include "driverChipInterface.h"






namespace {
/*
 * Step of the DriverChip.
 * The DriverChip will not tell us what step it is on.
 * We shadow it, i.e. keep track algorithmically.
 *
 * Not persistent and no initial value.
 * wake() sets the initial value, advancing it to match the motor.
 */
unsigned int shadowMicrostepOfDriver;


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
 */
#pragma PERSISTENT
unsigned int shadowMicrostepOfMotor;





void delayOneMilliSecond() { __delay_cycles(1000); }

void delayFiveMilliSecond() { __delay_cycles(5000); }

void delayTenMilliSecond() { __delay_cycles(10000); }

/*
 * Delay so motor turns at 100PPS (pulses per second).
 * For 20 step motor, that is 5 steps a second, or 300 RPM.
 *
 * This is called for each microstep.
 * PPS is for whole steps.
 * We are in half step mode.
 * So each delay is half of the delay for one pulse.
 */
void delaySo100PPS() {
    delayFiveMilliSecond();
}


}






void StepperIndexer::rememberMotorStep(){
    shadowMicrostepOfMotor = shadowMicrostepOfDriver;
}


void StepperIndexer::setShadowMicrostepOfDriver(unsigned int aStep) {
    shadowMicrostepOfDriver = aStep;
}


// quietly => non-energized
void StepperIndexer::restoreDriverToMotorStep() {
    // assert motor is on a detentStep
    // assert driverchip is wake, in fact just waked from sleep and reset

    // 2 is the state after reset when half step mode
    StepperIndexer::setShadowMicrostepOfDriver(2);

    // Prevent motor movement
    DriverChipInterface::disableCoilDrive();

    while ( not (shadowMicrostepOfDriver == shadowMicrostepOfMotor) ) {
        /*
         * Here we don't need a delay for speed, since motor is not turning, but we do need update shadow.
         */
        fastStepDetent();
    }

    DriverChipInterface::enableCoilDrive();
}



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
void StepperIndexer::delayAccordingToSpeed() {
    //delayOneMilliSecond();
    //delayOneMilliSecond();
    //delayOneMilliSecond();
    delaySo100PPS();
}


void StepperIndexer::stepMicrostep() {

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

    maintainShadowStep();
}



/*
 * Shadow the driver's microstep index.
 *
 * Modulo 8, for half step microstepping
 */
// TODO other step modes
void StepperIndexer::maintainShadowStep() {
    // assert we just microstepped twice (one detentStep)
    switch(DriverChipInterface::getDirection()) {
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
