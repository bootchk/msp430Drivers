
#include "led.h"


// DriverLib
#include <gpio.h>

#include "../board.h"




void LED::configureLED1() {

    // Ensure known state BEFORE configuring as out, else might blink
    // OUT register resets to "undefined".
    // Ensure known state
    turnOff();

	GPIO_setAsOutputPin(APP_LED1_PORT,    APP_LED1_PIN);

}

void LED::configureLED2() {
    turnOffLED2();
    GPIO_setAsOutputPin(APP_LED2_PORT,    APP_LED2_PIN);
}



/*
 * mcu sources current to LED: high is on
 */
void LED::turnOff(){
#ifdef LED_SOURCED
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#else
	GPIO_setOutputHighOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#endif
}

void LED::turnOffLED2() {
#ifdef LED_SOURCED
    GPIO_setOutputLowOnPin(APP_LED2_PORT,    APP_LED2_PIN);
#else
    GPIO_setOutputHighOnPin(APP_LED2_PORT,    APP_LED2_PIN);
#endif
}



void LED::turnOn(){
#ifdef LED_SOURCED
	GPIO_setOutputHighOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#else
	GPIO_setOutputLowOnPin(APP_LED1_PORT,    APP_LED1_PIN);
#endif
}


void LED::toggle() {
    GPIO_toggleOutputOnPin(APP_LED1_PORT, APP_LED1_PIN);
}

void LED::blink() {
    turnOn();
    // 100k cycles is about 1/100 Sec or 10 mSec
    __delay_cycles(100000);
    turnOff();
}
