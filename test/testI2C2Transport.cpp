
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

//#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

// Use the simplified interface to I2C
#include "../src/bridge/serialBus/i2c/i2cDirect.h"

// Not direct
#include "../src/bridge/serialBus/i2c/i2cTransport.h"

//#include "../src/pinFunction/i2cPins.h"




// DriverLib
#include <pmm.h>


void delayHalfSecond3() {
    __delay_cycles(500000); // half second
}

/*
 * Test I2C to external RTC via transport layer
 *
 * not using bridge but using transport layer
 *
 * In a loop, read time and toggle LED on Launchpad.
 * Expect: LED to blink forever.
 *
 * Exception: if something hangs, LED will stop blinking.
 * Driver not now designed to timeout if the MCU EUSCIB peripheral hangs.
 *
 * Exception: RTC is dead.
 * Driver will return an invalid time and softfault, which ???
 *
 * Does not test configuration of RTC.
 *
 * More testing of RTC is in test of Alarm
 */
void testI2CTransport()
{
    PMM_unlockLPM5();

    // RTC takes at least 0.5 seconds after power on before I2C is ready.
    delayHalfSecond3();
    delayHalfSecond3();

//#define TEMP

    left it not working to use I2CTransport::init
#ifdef TEMP    // Init peripheral
    I2CTransport::initI2CPeripheral();
    // pins not configured
    // not enabled

    I2CTransport::configurePins();

    I2CTransport::enable();
#else
    I2CDirect::init();
    // Pins are configured
    // peripheral enabled

    I2CDirect::setSlaveAddress(0x69);


    // TODO peripheral enabled a standard way
    // the current check isInitialized is foobar
#endif


#define USE_LED

#ifdef USE_LED
    LED::configureLED1();
#endif


    unsigned char buf[8];

    while (true)
    {
        delayHalfSecond3();

#ifdef USE_LED
        LED::toggle();
#endif

        // Test reading the time
        // TODO I2CDirect::readTime();

        // Test reading ID
        I2CTransport::read(0x28, buf, 1);

    }
}
