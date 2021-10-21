
#include "../src/LED/led.h"
#include <pmm.h>    // PMM_unlockLPM5
#include <src/pressureSensor/MPRLS.h>


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
 *
 * When tubing disconnected (ambient) expect 14.6 psi
 * When sucking hard on tube with mouth, expect about 7 psi
 * When blowing hard on tube with mouth, expect about 16 psi
 * When vacuum pump 3V KOGO micro vacuum pump and a foot of tubing, expect 8 psi
 *
 * When vacuum pump 3V KOGO micro vacuum pump and a foot of tubing,
 * and an hypodermic needle gauge 18, green luer
 * and tubing stuck into needle housing stuck into tubing (not good seal, needle should be inside tubing.)
 * open expect 14 psi
 * closed with finger 8.4 psi
 * closed with turnip seed expect 12-14 psi
 *
 * With needle (luer cut away to a ferrule only) inside tubing
 * closed with turnip seed expect 10.6 to 13.5 psi
 */

uint32_t data;

float pressure;

void testPressureSensor()
{
    PMM_unlockLPM5();

    LED::configureLED1();

    MPRLS::begin();

    // Ensure enough time elapsed for device to be ready

    while (true)
    {
        // uint8_t status = MPRLS::readStatus();
        data = MPRLS::readRawPressure();

        pressure = MPRLS::readPressure();

        LED::toggleLED1();
        __delay_cycles(1000000);
    }
}
