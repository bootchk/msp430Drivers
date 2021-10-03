
#include "../src/LED/led.h"
#include "../src/PressureSensor/MPRLS.h"




/*
 * Test MPRLS, an I2C pressure sensot.
 *
 *
 * Define constants in board.h to configure.
 *
 * Download and run.
 *
 * Expect red LED blinks as it measures.
 *
 * Set breakpoints at measurement cycle.
 */


void testPressureSensor()
{
    MPRLS mprls = MPRLS();

    LED::configureLED1();

    mprls.begin();

    while (true)
    {
        // uint8_t status = mprls.readStatus();
        uint32_t data = mprls.readRawPressure();

        LED::toggle();
        __delay_cycles(1000000);
    }
}
