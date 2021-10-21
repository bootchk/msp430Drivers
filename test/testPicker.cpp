
/*
 * Test prototype picker.
 *
 * pneumo valve,
 *
 *
 */

// DriverLib
#include <pmm.h>
#include <gpio.h>

#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"





// Proto has:

// stepper
#include "../src/stepperMotor/stepperMotor.h"
// MPRLS pressure sensor on I2C bus
#include "../src/pressureSensor/MPRLS.h"
// switch to a pneumo valve
#include "../src/lowSideSwitch/lowSideSwitch.h"


// To exam using debugger
static float failedPressure;


// configure all GPIO out (to ensure low power)
void configureAllGPIOOut() {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3.
    // Datasheet says only 3 pins??
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);
}

void setAllOutputsLow3() {
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;
}




void
delayBetweenTests3() {
    Delay::oneSecond();
    Delay::oneSecond();
}


/*
 * Arm
 */

void initArm() {
    /*
     * require is awake.
     * Either always awake and 1mS since power on,
     * or signaled awake and 1mS since signaled.
     */


    // Arm to home position.
    StepperMotor::findPhysicalStop(MotorDirection::Backward);
    // arm is against stop

    // For 20 step motor, 18 degrees per step, turn 54 degrees
    StepperMotor::turnAcceleratedStepsAndHold(3, MotorDirection::Forward);
    // arm is upright
}

void lowerArmIntoBin() {
    StepperMotor::turnAcceleratedStepsAndHold(3, MotorDirection::Backward);
}

void peckArm() {
    StepperMotor::turnAndHold(1, MotorDirection::Forward);
    // arm slightly lifted
    StepperMotor::turnAndHold(1, MotorDirection::Backward);
    // arm back in bin
}

void raiseArm() {
    StepperMotor::turnAcceleratedStepsAndHold(3, MotorDirection::Forward);
}




/*
 * Vacuum detection
 */

// with vacuum pump off, is 14.6
#define PRESSURE_THRESHOLD_LOW_VACUUM 14.55
// with vacumm pump on and open pickup tube, 14.5
#define PRESSURE_THRESHOLD_HIGH_VACUUM 14
// with vacuum pump on and object picked, blocking pickup tube < 14





bool
waitForPressureAbove(float threshold) {
    int   count;
   float pressure;

   count = 20; // gives a wait of > 100mX
   pressure = MPRLS::readPressure();
   while (pressure < threshold ) {
       // Pressure at pickup is just ambient (14.6), no vacuum (14.0)
       count--;
       if (count <0) {
           failedPressure = pressure;
           return false;
       }
       pressure = MPRLS::readPressure();
   }

   // pressure is above threshold
   return true;
}

bool
waitForPressureBelow(float threshold) {
   int   count;
   float pressure;

   count = 20; // gives a wait of > 100mX
   pressure = MPRLS::readPressure();
   while (pressure > threshold ) {
       // Pressure at pickup is just ambient (14.6), no vacuum (14.0)
       count--;
       if (count <0) {
           failedPressure = pressure;
           return false;
       }
       pressure = MPRLS::readPressure();
   }

   // pressure is below threshold
   return true;
}


bool
performActionUntilPressureBelow(float threshold, void func(void) ) {
    int   count;
    float pressure;

    count = 20; // gives a wait of > 100mX
    pressure = MPRLS::readPressure();

    while (pressure >threshold) {
        count--;
        if (count <0 ) {
            failedPressure = pressure;
            return false;
        }

        // Pressure at pickup is just a soft vacuum (14.0)
        func();
        // Reread pressure
        pressure = MPRLS::readPressure();
        }
    return true;
}

#ifdef OLD
/*
 * Wait for vacuum less than ambient and more than picked.
 */
bool
waitForUnpickedVacuum() {
    if ( ! waitForPressureAbove( MAX_UNPICKED_PRESSURE ) {
            return false;
    }

    // TODO check is not too low

    return true;
}


/*
 * Wait for vacuum more than picked.
 *
 * Not require reach ambient,
 * Require enough pressure to think that picking tube is not blocked by picked object or foreign object.
 */
bool
waitForUnpickedPressure() {
    while (pressure < RISING_UNPICKED_PRESSURE) {
        // Vacuum has not dropped
        pressure = MPRLS::readPressure();
        // assert 5mS has passed
    }
}
#endif



/*
 * Pneumo valve ops.
 */

void
pneumoValveConfigure() {
    LowSideSwitch::configure();
}
void
pneumoValveVacuumToCommon() {
    LowSideSwitch::turnOn();
}
void
pneumoValveAmbientToCommon() {
    LowSideSwitch::turnOff();
}



/*
 * Evemts and faults.
 */

void
configureFaultIndicators() {
    LED::configureLED1();
    LED::configureLED2();
}

void
faultVacuum() {
    /*
     * No vacuum on pickup.
     * vacuum pump or valve failed
     */
    // TODO sound alarm
    myAssert(false);
}

void
faultDropping() {
    /*
     * Object not dropped
     * or pickup tube is blocked
     */
    LED::turnOnLED1();

    // TODO if count exceeded, stop
    myAssert(false);
}

void
clearFaultDropping() {
    // TODO LED off
    // TODO reset count
}

void
faultPicking() {
    // Failed to pick object
    // TODO set LED
    // check count
    myAssert(false);
}

void clearFaultPicking() {
    // TODO
}






static void
testPecking() {
    lowerArmIntoBin();
    while(true) {
        peckArm();
    }
}


void
testPicker() {

    setAllOutputsLow3();
    configureAllGPIOOut();

    configureFaultIndicators();

    PMM_unlockLPM5();

    pneumoValveConfigure();

    // Pressure sensor
    MPRLS::begin(MPRLS_DEFAULT_ADDR);

    // HW should be keeping stepper driver chip awake
    StepperMotor::delayUntilDriverChipAwake();

    initArm();
    // Expect arm to move and find stop

    // Vacuum pump is always on

    // Pneumo valve is usually closed, giving mild vacuum to the pickup

    //testPecking();

    while(true) {

        // Vacuum applied to sensor and pickup tube
        pneumoValveVacuumToCommon();

        if ( !waitForPressureBelow(PRESSURE_THRESHOLD_LOW_VACUUM) )
            // Does not return
            faultVacuum();

        /*
         * Ensure pickup tube is not blocked.
         * Although we think we dropped an object,
         * another object could have been picked up, or dust accumulated.
         */
        if ( ! waitForPressureAbove(PRESSURE_THRESHOLD_HIGH_VACUUM) )
            // Might not return
            faultDropping();
        else
            clearFaultDropping();

        lowerArmIntoBin();

        // While not suction because seed is attached, peck
        if ( ! performActionUntilPressureBelow(PRESSURE_THRESHOLD_HIGH_VACUUM, peckArm) )
            // Might not return
            faultPicking();
        else
            clearFaultPicking();

        raiseArm();

        // Stop sucking, ambient pressure to pickup
        // Expect picked object to drop
        pneumoValveAmbientToCommon();

        /*
         * We don't here waitForPressureAbove(PRESSURE_THRESHOLD_HIGH_VACUUM).
         * Since we will soon check at the top of the loop.
         *
         * That is, we don't here check that the object dropped.
         */

        delayBetweenTests3();
    }
}
