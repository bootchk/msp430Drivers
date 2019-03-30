
#include "blinkingLED.h"

// implementation
#include "led.h"


// 100k cycles at 1Mhz clock is about 1/10 Sec


#ifdef SPIN_BLINK_LED

void BlinkingLED::blinkFirst() {
    LED::turnOn();
    __delay_cycles(100000);
    LED::turnOff();
}

void BlinkingLED::blinkSecond() {
    LED::turnOnLED2();
    __delay_cycles(100000);
    LED::turnOffLED2();
}

#else

#include "../timer/timer.h"

void BlinkingLED::blinkFirst() {
    LED::turnOn();
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOff();
}

void BlinkingLED::blinkSecond() {
    LED::turnOnLED2();
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOffLED2();
}

#endif

