
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

    StepperIndexer::setShadowMicrostepOfDriver(2);

    // Prevent motor movement
    DriverChipInterface::disableOutput();

    while ( not (shadowMicrostepOfDriver == shadowMicrostepOfMotor) ) {
        /*
         * Here we don't need a delay for speed, since motor is not turning, but we do need update shadow.
         */
        fastStepDetent();
    }

    DriverChipInterface::enableOutput();
}




void StepperIndexer::delayAccordingToSpeed() {
    delayOneMilliSecond();
    delayOneMilliSecond();
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
        shadowMicrostepOfDriver += 2;
        if (shadowMicrostepOfDriver > 7)
                shadowMicrostepOfDriver = 0;
        break;
    case MotorDirection::Backward:
        if (shadowMicrostepOfDriver == 0)
                shadowMicrostepOfDriver = 8;
        shadowMicrostepOfDriver -= 2;
        break;
    }
    // WAS assert shadowMicrostepOfDriver in [0..7]
    // assert shadowMicrostepOfDriver in [0, 2, 4, 6]
}
