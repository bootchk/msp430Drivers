
#include "../src/LED/led.h"
#include "../src/PressureSensor/MPRLS.h"

// TI DriverLib
#include <pmm.h>    // PMM_unlockLPM5


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

uint32_t data;

void testPressureSensor()
{
    PMM_unlockLPM5();

    MPRLS mprls = MPRLS();

    LED::configureLED1();

    mprls.begin();

    // Ensure enough time elapsed for device to be ready

    while (true)
    {
        // uint8_t status = mprls.readStatus();
        data = mprls.readRawPressure();

        LED::toggle();
        __delay_cycles(1000000);
    }
}
