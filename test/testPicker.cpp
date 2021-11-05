
/*
 * Test prototype picker.
 */

// DriverLib
#include <pmm.h>
#include <gpio.h>

#include "../src/delay/delay.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"





// Proto has:

// stepper
#include "../src/stepperMotor/degreeStepperMotor.h"
// MPRLS pressure sensor on I2C bus
#include "../src/pressureSensor/MPRLS.h"
// switch to a pneumo valve
#include "../src/lowSideSwitch/lowSideSwitch.h"


// To exam using debugger
static float failedPressure;

/*
 * Calculated once at startup.
 * Constant for duration of session.
 */
static float AmbientPressure;
static float PressureThresholdLowVacuum;
static float PressureThresholdHighVacuum;



static unsigned int consecutiveCountFaultDropping = 0;
static unsigned int consecutiveCountFaultPicking = 0;

/*
 *  Count times to read pressure before declaring unrecoverable fault.
 *
 *  5mS per read, so the delay is about 50mS
 */
#define PressureReadsPerUnrecoverableFault 10

/*
 * Count times to read pressure before declaring recoverable fault.
 *
 * Operation continues.
 * May result in a skipped pick.
 */
#define PressureReadsPerRecoverableFault 10


/*
 * How many recoverable faults, consecutively,
 * justify operator intervention.
 */
#define ConsecutiveRecoverableFaultsPerAbort 3


/*
 * How many full steps in a dip and raise of the arm.
 *
 * 3 * 18 degrees == 54 degrees
 * 4 * 18 degrees == 72 degrees
 */
#define StepsPerArmDip   4
#define DegreesPerArmDip 54


#define DegreesUprightToAboveBin  54

#define JiggleDegrees             18

#ifdef OLD
#define MicrostepsLoweringIntoSeeds 4   // Quarter microsteps
#endif

// For 200 step motor, to get 18 degrees
#define StepsLoweringIntoSeeds      10


/*
 * From CCW stop to home (upright)
 */
#define StepsToHomeFromCCWStop    1
#define DegreesToHomeFromCCWStop  18


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







/*
 * Arm
 */

// Wiggle the arm to help visually distinguish forward and back
void
testArmMotion()
{
    DegreeStepperMotor::turnAndHoldDegrees(18, MotorDirection::Backward, MotorSpeed::Quarter);
    Delay::oneSecond();
    DegreeStepperMotor::turnAndHoldDegrees(18, MotorDirection::Forward, MotorSpeed::Quarter);
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    // Max speed
    DegreeStepperMotor::turnAndHoldDegrees(18, MotorDirection::Backward, MotorSpeed::Max);
    Delay::oneSecond();
    DegreeStepperMotor::turnAndHoldDegrees(18, MotorDirection::Forward, MotorSpeed::Max);
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    // 90 degrees
    DegreeStepperMotor::turnAndHoldDegrees(90, MotorDirection::Backward, MotorSpeed::Quarter);
    Delay::oneSecond();
    DegreeStepperMotor::turnAndHoldDegrees(90, MotorDirection::Forward, MotorSpeed::Quarter);
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    // Max speed
    DegreeStepperMotor::turnAndHoldDegrees(90, MotorDirection::Backward, MotorSpeed::Max);
    Delay::oneSecond();
    DegreeStepperMotor::turnAndHoldDegrees(90, MotorDirection::Forward, MotorSpeed::Max);
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();



    Delay::oneSecond();
}

void
initArmToHome() {
    // We don't need to find stop CW
    //DegreeStepperMotor::findPhysicalStopAndHold(MotorDirection::Backward, MotorSpeed::Half);
    //Delay::oneSecond();

    // Forward is CCW
    DegreeStepperMotor::findPhysicalStopAndHold(MotorDirection::Forward, MotorSpeed::Half);
    // arm is against stop, which is certain degrees CCW of upright.
    Delay::oneSecond();

    // StepperMotor::turnAcceleratedStepsAndHold(1, MotorDirection::Backward);
    // TODO define constant
    DegreeStepperMotor::turnAndHoldDegrees(18, MotorDirection::Backward, MotorSpeed::Half);
    // arm is upright
    Delay::oneSecond();
}


/*
 * Used to find home when the bin is the clockwise stop.
 */
void
initArmToClockwiseStop() {
    /*
     * require is awake.
     * Either always awake and 1mS since power on,
     * or signaled awake and 1mS since signaled.
     */


    DegreeStepperMotor::findPhysicalStopAndHold(MotorDirection::Backward, MotorSpeed::Max);
    // arm is against stop

    // StepperMotor::turnAcceleratedStepsAndHold(StepsPerArmDip, MotorDirection::Forward);
    DegreeStepperMotor::turnAndHoldDegrees(DegreesPerArmDip, MotorDirection::Forward, MotorSpeed::Quarter);
    // arm is upright
}

/*
 * Used to find home at certain degrees from counter clockwise stop.
 */
void initArmToCounterClockwiseStop() {
    // Forward is CCW
    DegreeStepperMotor::findPhysicalStopAndHold(MotorDirection::Forward, MotorSpeed::Max);
    // arm is against stop

    // For 20 step motor, 18 degrees per step, turn 18 degrees CW
    // StepperMotor::turnAcceleratedStepsAndHold(StepsToHomeFromCCWStop, MotorDirection::Backward);
    DegreeStepperMotor::turnAndHoldDegrees(DegreesToHomeFromCCWStop, MotorDirection::Backward, MotorSpeed::Quarter);
    // arm is upright
}

void lowerArmIntoBin() {
    DegreeStepperMotor::turnAndHoldDegrees(DegreesPerArmDip, MotorDirection::Backward, MotorSpeed::Quarter);
    //StepperMotor::turnStepsDirectionSpeedAndHold(StepsPerArmDip, MotorDirection::Backward, MotorSpeed::Sixteenth);
    //StepperMotor::turnAcceleratedStepsAndHold(StepsPerArmDip, MotorDirection::Backward);
}

void raiseArm() {
    DegreeStepperMotor::turnAndHoldDegrees(DegreesPerArmDip, MotorDirection::Forward, MotorSpeed::Quarter);
    // StepperMotor::turnStepsDirectionSpeedAndHold(StepsPerArmDip, MotorDirection::Forward, MotorSpeed::Sixteenth);
    // StepperMotor::turnAcceleratedStepsAndHold(StepsPerArmDip, MotorDirection::Forward);
}

// Lower but not into the bin, certain degrees short
void
lowerArmToAboveBin() {
    DegreeStepperMotor::turnAndHoldDegrees(DegreesUprightToAboveBin, MotorDirection::Backward, MotorSpeed::Quarter);
    // StepperMotor::turnStepsDirectionSpeedAndHold(StepsPerArmDip -1, MotorDirection::Backward, MotorSpeed::Sixteenth);
}


void peckArm() {
#ifdef OLD
    DegreeStepperMotor::turnAndHold(1, MotorDirection::Forward);
    // arm slightly lifted
    DegreeStepperMotor::turnAndHold(1, MotorDirection::Backward);
    // arm back in bin
#endif
    myAssert(false);    // unimplemented
}



/*
 * A jiggle is 18 degrees in opposite directions
 */
void jiggleArm() {
    //StepperMotor::jiggle();
    DegreeStepperMotor::turnAndHoldDegrees(JiggleDegrees, MotorDirection::Forward, MotorSpeed::Quarter);
    DegreeStepperMotor::turnAndHoldDegrees(JiggleDegrees, MotorDirection::Backward, MotorSpeed::Quarter);
}

void reverseJiggleArm() {
    // StepperMotor::reverseJiggle();
    DegreeStepperMotor::turnAndHoldDegrees(JiggleDegrees, MotorDirection::Backward, MotorSpeed::Quarter);
    DegreeStepperMotor::turnAndHoldDegrees(JiggleDegrees, MotorDirection::Forward, MotorSpeed::Quarter);
}







/*
 * Vacuum detection
 */

/*
 * With vacuum pump off, average ambient is about 14.6
 *
 * At sea level, barometric pressure range is [29,31] Hg (inches of mercury
 * which is [14.24, 15.22] psi
 *
 * We defined thresholds relative to ambient
 */
/*
 * With pump on and unblocked pickup tube, vacuum drops more than 0.2 from ambient
 */
#define PRESSURE_DIFFERENTIAL_LOW_VACUUM 0.2

/*
 * with vacuum pump on and blocked pickup tube, vacuum drops more than 0.6 from ambient
 */
#define PRESSURE_DIFFERENTIAL_HIGH_VACUUM 0.6


void calculatePressureThresholds() {

    // assert vacuum pump is off
    AmbientPressure = MPRLS::readPressure();

    PressureThresholdLowVacuum  = AmbientPressure - PRESSURE_DIFFERENTIAL_LOW_VACUUM;
    PressureThresholdHighVacuum = AmbientPressure - PRESSURE_DIFFERENTIAL_HIGH_VACUUM;
}


bool
waitForPressureAbove(
        unsigned int maxAttempts,
        float        threshold)
{
   unsigned int   countAttempts;
   float          pressure;

   countAttempts = maxAttempts;
   pressure = MPRLS::readPressure();
   while (pressure < threshold ) {
       // Pressure at pickup is just ambient (14.6), no vacuum (14.0)
       countAttempts--;
       if (countAttempts == 0) {
           failedPressure = pressure;
           return false;
       }
       pressure = MPRLS::readPressure();
   }

   // pressure is above threshold
   return true;
}

bool
waitForPressureBelow(
        unsigned int maxAttempts,
        float threshold)
{
    unsigned int   countAttempts;
    float          pressure;

   countAttempts = maxAttempts;
   pressure = MPRLS::readPressure();
   while (pressure > threshold ) {
       // Pressure at pickup is just ambient (14.6), no vacuum (14.0)
       countAttempts--;
       if (countAttempts == 0) {
           failedPressure = pressure;
           return false;
       }
       pressure = MPRLS::readPressure();
   }

   // pressure is below threshold
   return true;
}


bool
performActionUntilPressureBelow(
        unsigned int maxAttempts,
        float        threshold,
        void         func(void) )
{
    int   countAttempts;
    float pressure;

    countAttempts = maxAttempts;
    pressure = MPRLS::readPressure();

    while (pressure > threshold) {
        countAttempts--;
        if (countAttempts == 0 ) {
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


bool
performActionUntilPressureAbove(
        unsigned int maxAttempts,
        float        threshold,
        void         func(void) )
{
    int   countAttempts;
    float pressure;

    countAttempts = maxAttempts;
    pressure = MPRLS::readPressure();

    while (pressure < threshold) {
        countAttempts--;
        if (countAttempts == 0 ) {
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
 * Events and faults.
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

    // both LEDs
    LED::turnOnLED1();
    LED::turnOnLED2();
    myAssert(false);
}

void
faultDropping() {
    /*
     * Object not dropped
     * or pickup tube is blocked
     */
    // Red
    LED::turnOnLED1();

    consecutiveCountFaultDropping += 1;
    if (consecutiveCountFaultDropping > ConsecutiveRecoverableFaultsPerAbort) {
        myAssert(false);
    }
}

void
clearFaultDropping() {
    LED::turnOffLED1();
    consecutiveCountFaultDropping = 0;
}

void
faultPicking() {
    // Failed to pick object
    // green LED on launchpad
    LED::turnOnLED2();

    consecutiveCountFaultPicking += 1;
    if (consecutiveCountFaultPicking > ConsecutiveRecoverableFaultsPerAbort) {
        myAssert(false);
    }
}

void clearFaultPicking() {
    LED::turnOffLED2();
    consecutiveCountFaultPicking = 0;
}





/*
 * Phases of action.
 *
 * Side effects on faults
 */
void
jiggleWhileNotSeedAttached() {
    if ( ! performActionUntilPressureBelow(
            PressureReadsPerRecoverableFault,
            PressureThresholdHighVacuum,
            jiggleArm) )
        // Might not return
        faultPicking();
    else
        clearFaultPicking();
}

void
ensurePickupTubeNotBlocked() {

    if ( ! performActionUntilPressureAbove(
                    PressureReadsPerRecoverableFault,
                    PressureThresholdHighVacuum,
                    reverseJiggleArm) )
        // Might not return
        faultDropping();
        /*
         * We proceed to pick.  It may dislodge the stuck object and pick up a new one,
         * or it may think the stuck object is a successful pick.
         *
         * If it persists, we will abort.
         */
    else
        clearFaultDropping();
}



/*
 * Partial parameterization so signature of an action.
 */
#ifdef TODO

void microstepInto() {
    StepperMotor::turnAndHoldMicrosteps(1, MotorDirection::Backward);
}
#endif

void stepInto() {
    DegreeStepperMotor::turnAndHoldMinDegrees(MotorDirection::Backward, MotorSpeed::Quarter);
}

/*
 * Lower by  18 degrees while checking for vacuum.
 */
bool
lowerArmLastStepIntoBin() {
    bool result;
    result = performActionUntilPressureBelow(
                        StepsLoweringIntoSeeds,
                        PressureThresholdHighVacuum,
                        stepInto);
    // Might be deep in the seeds if no seed attached yet
    // Result: is seed attached?
    return result;
}


//Partial test.

static void
testPecking() {
    lowerArmIntoBin();
    while(true) {
        peckArm();
    }
}


// Full test
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
    DegreeStepperMotor::delayUntilDriverChipAwake();

    calculatePressureThresholds();

    testArmMotion();

    initArmToHome();
    // Expect arm to move to home position: upright

    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();
    Delay::oneSecond();

    // TODO turn vacuum pump on

    // Pneumo valve is usually closed, giving mild vacuum to the pickup

    //testPecking();

    while(true) {

        // Vacuum applied to sensor and pickup tube
        pneumoValveVacuumToCommon();

        if ( !waitForPressureBelow(
                PressureReadsPerUnrecoverableFault,
                PressureThresholdLowVacuum ) )
            /*
             * Unrecoverable, requires operator intervention.
             * Does not return
             */
            faultVacuum();

        /*
         * Although we think we dropped an object,
         * another object could have been picked up, or dust accumulated.
         */
        ensurePickupTubeNotBlocked();

        //lowerArmIntoBin();

        lowerArmToAboveBin();
        if ( ! lowerArmLastStepIntoBin() )
            jiggleWhileNotSeedAttached();

        // Seed attached, or fault

        raiseArm();

        Delay::oneSecond();

        // Stop sucking, ambient pressure to pickup
        // Expect picked object to drop
        pneumoValveAmbientToCommon();

        /*
         * We don't here waitForPressureAbove(PRESSURE_THRESHOLD_HIGH_VACUUM).
         * Since we will soon check at the top of the loop.
         *
         * That is, we don't here check that the object dropped, we check a little later.
         */

        Delay::oneSecond();
        Delay::oneSecond();
        Delay::oneSecond();
    }
}

// TODO fault go back to home position, restart
