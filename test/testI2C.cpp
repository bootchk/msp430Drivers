
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

//#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

// Use transport layer interface to I2C
#include "../src/i2c/transport/i2cTransport.h"

//#include "../src/pinFunction/i2cPins.h"




// DriverLib
#include <pmm.h>


void delayHalfSecond2() {
    __delay_cycles(500000); // half second
}

/*
 * Test I2C to external RTC.
 *
 * More direct than other tests: not using bridge or transport layer
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
void testI2C()
{
    PMM_unlockLPM5();

    // RTC takes at least 0.5 seconds after power on before I2C is ready.
    delayHalfSecond2();
    delayHalfSecond2();

    // Init peripheral
    I2CTransport::initI2CPeripheral(0x69);
    I2CTransport::configurePinsWithExternalPullups();
    I2CTransport::enable();


    // TODO consolidate
    myAssert(I2CTransport::isInitialized);
    myAssert(I2CTransport::isConfiguredPinsForModule());
    myAssert(I2CTransport::isEnabled());


    // We configured pins in init()
    // These are alternate methods, using more sophisticated library
    // Bridge::configureMcuSide(false);
    // OR I2CTransport::configurePins();
    // I2CPins::configureWithInternalPullup();


#define USE_LED

#ifdef USE_LED
    LED::configureLED1();
#endif

    while (true)
    {
        delayHalfSecond2();

#ifdef USE_LED
        LED::toggle();
#endif

        // Test reading the time
        static unsigned char buf[7];

        I2CTransport::read(0x0, buf, 7);
        //I2CDirect::readTime();

        // Test reading ID
        myAssert(RTC::isReadable());
        // equivalent to unsigned int ID = I2CDirect::readID();

        // TODO test writing something

    }
}
