
/*
 * The part of StepperIndexer related to step shadowing.
 */


#include "stepperIndexer.h"

// Uses chip driver
#include "driverChipInterface.h"



// DriverLib
// #include <gpio.h>





namespace {
/*
 * Step of the DriverChip.
 * The DriverChip will not tell us what step it is on.
 * We shadow it, i.e. keep track algorithmically.
 *
 * Initial value is the HomeStep.
 * Newly waked DriverChip resets to HomeStep.
 */
unsigned int shadowStepOfDriver = 2;


/*
 * Step the motor is on.
 *
 * We don't know this until we sync the motor with the DriverChip.
 * Then we assume the motor stays in sync (we don't keep it updated while stepping motor.)
 * We remember this when we sleep.
 * When we wake, the shadowStepOfDriver is reset to the HomeStep,
 * and we advance shadowStepOfDriver to match the remembered stepOfMotor.
 */
unsigned int stepOfMotor;



void delayOneMilliSecond() { __delay_cycles(1000); }



}







void StepperIndexer::rememberMotorStep(){
    stepOfMotor = shadowStepOfDriver;
}




void StepperIndexer::setShadowStepOfDriver(unsigned int aStep) {
    shadowStepOfDriver = aStep;
}


// quietly => non-energized
void StepperIndexer::restoreDriverToMotorStep() {

    StepperIndexer::setShadowStepOfDriver(2);

    DriverChipInterface::disableOutput();

    while ( not (shadowStepOfDriver == stepOfMotor) ) {
        /*
         * Here we don't need a delay for speed, since motor is not turning, but we do need update shadow.
         */
        // TODO eliminate the speed delay
        stepMicrostep();
    }

    DriverChipInterface::enableOutput();
}







void StepperIndexer::stepMicrostep() {

    // tell driver
    DriverChipInterface::stepMicrostep();

    delayAccordingToSpeed();

    /*
     * Shadow the driver's microstep index.
     */
   shadowStepOfDriver++;

   // Modulo
   // TODO according to step mode
   // this is for half step
   if (shadowStepOfDriver > 7 ) shadowStepOfDriver = 0;
}




// TODO this for half
void StepperIndexer::stepDetent() {
    // !!! Call to self, not to DriverChipInterface
    stepMicrostep();
    stepMicrostep();
}



/*
 * sleep/wake
 */
void StepperIndexer::wake() {

    DriverChipInterface::wake();

    // assert DriverChip in state 2
    // assert motor is at remembered motor step

    restoreDriverToMotorStep();
    // assert DriverChip is at same step as motor
}


void StepperIndexer::sleep() {
    DriverChipInterface::sleep();

    rememberMotorStep();
}







void StepperIndexer::delayAccordingToSpeed() {
    delayOneMilliSecond();
    delayOneMilliSecond();
}


