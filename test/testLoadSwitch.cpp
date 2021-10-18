
/*
 * Test a breakout board having an NMOS FET to switch low side of 5V solenoid.
 *
 * If the solenoid is a pneumo valve, expect 2 seconds of open, then 2 seconds of closed.
 */

// DriverLib
#include <pmm.h>
#include <gpio.h>

#include "../src/delay/delay.h"




// configure all GPIO out to ensure low power
void configureGPIOLowPower2() {
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

void setAllOutputsLow2() {
    P1OUT = 0x00; P2OUT = 0x00; P3OUT = 0x00;
}




void
delayBetweenTests2() {
    Delay::oneSecond();
    Delay::oneSecond();
}




void
testLoadSwitch() {

    setAllOutputsLow2();
    configureGPIOLowPower2();

    PMM_unlockLPM5();


    while(true) {

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
        delayBetweenTests2();


        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
        delayBetweenTests2();
    }
}
