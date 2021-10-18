
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
#include "../src/lowSideSwitch/lowSideSwitch.h"



// Proto has:

// stepper
#include "../src/stepperMotor/stepperMotor.h"




// configure all GPIO out to ensure low power
void configureGPIOLowPower3() {
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




void
testPicker() {

    setAllOutputsLow3();
    configureGPIOLowPower3();
    PMM_unlockLPM5();


    // Pneumo valve
    LowSideSwitch::configure();

    // Pressure sensor
    // TODO


    // Arm to home position.
    StepperMotor::findPhysicalStop(MotorDirection::Backward);
    // arm is against stop

    // For 20 step motor, 18 degrees per step, turn 54 degrees
    StepperMotor::turnAcceleratedStepsAndHold(3, MotorDirection::Forward);
    // arm is upright


    // Vacuum is always on

    // TODO test that pneumo valve is closed and vacuum exists


    while(true) {

        // Vacuum on
        LowSideSwitch::turnOff();

        // lower arm into bin
        // TODO

        // If not suction, peck
        // TODO

        // raise arm
        // TODO

        // vacuum off
        // Expect picked object to drop
        LowSideSwitch::turnOn();

        // Wait for loss of vacuum (pressure is ambient)

        delayBetweenTests3();
    }
}
