

#include "../src/LED/led.h"

#include "../src/delay/delay.h"

/*
 * Test LED will blink.
 *
 * A very basic test that your dev system is working.
 *
 * Define constants in board.h to configure
 * to use the LED's on your Launchpad
 *
 * Download and run.
 * Expect first LED (red) on the LP blinks every second.
 */


void testLED()
{

    LED::configureLED1();

    while (true)
    {
        LED::toggleLED1();
        Delay::oneSecond();
    }
}
