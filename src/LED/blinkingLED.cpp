
#include "blinkingLED.h"

// implementation
#include "led.h"


// 100k cycles at 1Mhz clock is about 1/10 Sec
#define TenthSecondAt1Mhz  100000

#ifdef SPIN_BLINK_LED

void BlinkingLED::blinkFirst() {
    LED::turnOn();
    __delay_cycles(TenthSecondAt1Mhz);
    LED::turnOff();
}

void BlinkingLED::blinkSecond() {
    LED::turnOnLED2();
    __delay_cycles(TenthSecondAt1Mhz);
    LED::turnOffLED2();
}

#else

// low power mode while LED is on

#include "../timer/timer.h"

void BlinkingLED::blinkFirst() {
    LED::configureLED1();
    LED::turnOnLED1();
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOffLED1();
}

void BlinkingLED::blinkSecond() {
    LED::turnOnLED2();
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOffLED2();
}

#endif

