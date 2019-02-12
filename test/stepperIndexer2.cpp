
/*
 * The part of StepperIndexer related to step shadowing.
 */


#include "stepperIndexer.h"

// DriverLib
#include <gpio.h>





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

    disableOutput();

    while ( not (shadowStepOfDriver == stepOfMotor) ) microstep();

    enableOutput();
}







void StepperIndexer::microstep() {
    /*
     * Pulse high the "step" pin.
     * If microstepping, not a full step.
     */
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // driver requires > 1.7uSec pulse high
    // assume procedure call overhead is enough
    //__delay_cycles(2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    // driver requires > 1.7uSec pulse low
    //__delay_cycles(2);

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
    microstep();
    microstep();
}
