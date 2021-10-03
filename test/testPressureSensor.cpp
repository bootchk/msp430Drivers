
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

    while (true)
    {
        mprls.begin();

        LED::toggle();
        __delay_cycles(1000000);
    }
}
