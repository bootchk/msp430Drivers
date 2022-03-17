
// DriverLib
#include <pmm.h>

#include "../src/pinFunction/allPins.h"
#include "../src/stepperMotor/stepperMotor.h"
#include "../src/stepperMotor/degreeStepperMotor.h"
#include <src/stepperIndexer/chipInterface/chipInterface.h>
#include "../src/stepperIndexer/stepperIndexer.h"
#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"
#include "../src/stepperMotor/standbyStepper.h"
#include "../src/timer/timer.h"
#include "../src/ADC/adc.h"
#include "../src/LED/led.h"


/*
 * For testing using power from debugger, use 320 (since it provides 3.3V)
 */
#define SURPLUS_VCC_VOLTAGE 330
/*
 * For testing using power from a separate power board (when debugger of Launchpad is not connected to MSP430)
 * use 350 (when that power board provides VCC ranging from 2 to 3.6V)
 */
//#define SURPLUS_VCC_VOLTAGE 350

/*
 * Vcc is from a supercapacitor via a regulator to 3.6V.
 * The supercapacitor is charged above 3.6V, say to 5.5V.
 * When the supercapacitor drops below 3.6V (say arbitrarily to 3.5V)
 * we know there is not an excess charge on the capacitor.
 * Not enough to continue a high load: stepping a motor.
 */
bool
isSurplusEnergy()
{
    unsigned int vccCentiVolts = ADC::measureVccCentiVolts();
    return (vccCentiVolts > SURPLUS_VCC_VOLTAGE);
}



void
delayInLowPowerMode()
{
    //  not low power
    //Delay::oneSecond();

    LowPowerTimer::delaySeconds(1);
}

// Not surplus is green LED, surplus is red LED.
void indicateSurplus()
{
    // Also light an LED, in case the motor movement is not sufficient to indicate.
    // In the future, we may turn the motor only ever so often.
    // In the future, the LED may be eInk, taking no power.
    LED::turnOnLED1();
    LED::turnOffLED2();
}

void indicateNotSurplus()
{
    LED::turnOnLED2();
    LED::turnOffLED1();
}





/*
 * Test stepping a motor,
 * where the motor driver is unpowered between steps,
 * and enter low power mode.
 */
void
testSunDial2() {

    AllPins::setAllOutputsLow();
    AllPins::configureGPIOLowPower();

    PMM_unlockLPM5();

    /*
     * Assert the high side load switch for the stepper driver is configured and off.
     */

    while (true) {
        if (isSurplusEnergy())
        {
            // Enough surplus to turn the motor.
            StandbyStepperMotor::powerOnAndStepThenOff();

            // Indicate that we might be turning the motor
            // Redundant, in case the motor does not actually turn.
            indicateSurplus();
        }
        else {
            // Else indicate that we are not trying to turn the motor.
            indicateNotSurplus();
        }

        /*
         * For testing purposes, we don't need to be in low power mode sleep.
         * But it will drain the storage supercap sooner.
         */
        delayInLowPowerMode();
    }
}
