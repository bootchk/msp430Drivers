#include "../src/alarm/alarm.h"

#include "../src/pinFunction/allPins.h"
#include "../src/SoC/SoC.h"
#include "../src/bridge/bridge.h"
#include "../src/LED/led.h"

#include "../src/assert/myAssert.h"

// DriverLib
#include <pmm.h>




/*
 * Test: multiple alarms with LPM4.5 in between.
 *
 * Here, we reconfigure Alarm after every sleep.
 *
 * Use EnergyTrace and expect one energy pulse every 10 seconds of sleep.
 */


void testAlarmLPM45()
{
    // assert watchdog stopped

    AllPins::setLowOutput();
    // assert all pin configs same, but not necessarily unlocked.
    // assert any pins on LaunchPad that source an LED, the LED will be dark

    Bridge::configureToSleepState();        // e.g. I2C bus pins to external RTC are now GPIO
    Alarm::configureMcuAlarmInterface();    // Alarm pin
    // Pins configured in sleep config, but not necessarily unlocked, and no interrupt is enabled.

    if ( SoC::isResetWakeFromSleep() ) {

        SoC::clearIFGForResetWakeFromSleep();
        SoC::unlockGPIOFromSleep();
        /*
         * HW would call ISR here, except we have not enabled interrupt on alarm,
         * so no ISR is actually called.
         */

        Alarm::configureAfterWake();
        // assert bus to RTC ready and RTC still configured
        Alarm::clearBothSidesOfSignal();

        LED::blinkLED2();
    }
    else {
        // Coldstart

        // Ensure unlocked, for certain debugging state where LPM5 is locked
        SoC::unlockGPIOFromSleep();

        // Configure bus to RTC and initialize RTC
        Alarm::configureAfterColdReset();

        LED::blink();   // red
    }

    // assert bus to RTC configured and RTC is configured
    // assert ready for setAlarm()
    const Duration duration = {3};
    Alarm ::setAlarmDurationSecondsFromNow(duration);

    /*
     * Restore pin config to sleeping state: alarm pin but no bus pins
     */
    Alarm::unconfigureMcuBusInterface();

    /*
     * Enter LPM4.5
     */
    PMMCTL0_H = PMMPW_H;        // Unlock PMM Registers
    PMMCTL0_L &= ~(SVSHE);      // Disable SVSH; saves more current
    PMMCTL0_L |= PMMREGOFF;     // Core regulator off; gives the ".5" aspect of LPMx.5 (cpu unpowered)
    PMMCTL0_H = 0;                    // Lock PMM Registers

    /*
     * Enter LPM4.  Since core regulator is off, this will be LPM4.5.  Since SVSH off, will be the lowest possible current in sleep.
     *
     * TI recommends as few instructions as possible between core regulator off and entering LPM4.
     * Since the core is running off bypass capacitance (not the LDO regulator) there is only enough energy for a few instructions.
     *
     * equivalent to:  __bis_SR_register(LPM4_bits | GIE);
     */
    _low_power_mode_4();
    /*
     * LPM4.5 does not return, sleep will end via a RESET event and continuation is at main().
     */
    myAssert(false);
}
