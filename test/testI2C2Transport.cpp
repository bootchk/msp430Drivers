
#include "../src/alarmClock/realTimeClock/realTimeClock.h"

//#include "../src/bridge/bridge.h"
#include "../src/assert/myAssert.h"
#include "../src/LED/led.h"

// Use the direct interface to I2C
//#include "../src/bridge/serialBus/i2c/i2cDirect.h"

// Not direct
#include "../src/bridge/serialBus/i2c/transport/i2cTransport.h"

//#include "../src/pinFunction/i2cPins.h"




// DriverLib
#include <pmm.h>

static unsigned int errorCount = 0;


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

#define INIT_USING_TRANSPORT


#ifdef INIT_USING_TRANSPORT
    // Init peripheral
    I2CTransport::initI2CPeripheral(0x69);
    // slave set
    // pins configured
    // data rate set
    // enabled

    //I2CTransport::setDataRate125kbps();

    // Choice
    I2CTransport::configurePinsWithExternalPullups();
    //I2CTransport::configurePinsWithInternalPullups();

    //I2CTransport::enable();

#else
    // init using direct
    // ??? only for external pullups???

    I2CDirect::init();
    // Pins are configured
    // peripheral enabled

    I2CDirect::setSlaveAddress(0x69);


    // TODO peripheral enabled a standard way
    // the current check isInitialized is foobar
#endif

myAssert(I2CTransport::isConfiguredPinsForModule());
// TODO assert data rate, slave, enabled, external pullups

#define USE_LED

#ifdef USE_LED
    LED::configureLED1();
#endif


    unsigned char buf[8];
    unsigned const char alarmBuf[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    while (true)
    {
        delayHalfSecond3();

#ifdef USE_LED
        LED::toggle();
#endif

        // Test reading ID
        if (I2CTransport::read(0x28, buf, 1)) {
            myAssert(buf[0] == 0x8);    // ID of RTC chip
        }
        else {
            errorCount++;
        }

        // Test reading the time
        if (not I2CTransport::read(0x0, buf, 8) ) errorCount++;
        // buf should be a time, buf[0] is hundredths, buf[1] is seconds, etc.

        // Test writing alarm
        if ( not I2CTransport::write(0x8, alarmBuf, 8) ) errorCount++;

        // Test by reading alarm just written
        if ( I2CTransport::read(0x8, buf, 8) ) {
            for (int i = 0; i<8; i++) {
                myAssert(buf[i] == alarmBuf[i]);
            }
        }
        else {
            errorCount++;
        }
    }
}
