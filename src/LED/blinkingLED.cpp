
#include "blinkingLED.h"

// implementation
#include "led.h"


// 100k cycles at 1Mhz clock is about 1/10 Sec
#define TenthSecondAt1Mhz  100000

// TODO move this to deviceConfig.h
#define SPIN_BLINK_LED

#ifdef SPIN_BLINK_LED

void BlinkingLED::blinkFirst() {
    LED::turnOnLED1();
    __delay_cycles(TenthSecondAt1Mhz);
    LED::turnOffLED1();
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
    // !!! The LPT may use the RTC: ensure not already used.
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOffLED1();
}

void BlinkingLED::blinkSecond() {
    LED::turnOnLED2();
    LowPowerTimer::delayTenMilliSeconds();
    LED::turnOffLED2();
}

#endif

