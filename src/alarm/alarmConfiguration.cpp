
#include "alarm.h"

#include "../bridge/bridge.h"       //  hides serial io
#include "../realTimeClock/realTimeClock.h"


#include "../pinFunction/alarmPin.h"    // hides GPIO functions


#include "../assert/myAssert.h"
#include "../PMM/powerMgtModule.h"  // for assertions only



/*
 * Part of implementation of Alarm.
 * Configuration aspect.
 */


/*
 * State variable that is NOT persistent across sleeps.
 */
bool _isConfiguredForAlarming = false;


bool Alarm::isConfiguredForAlarming() {
    return _isConfiguredForAlarming;
}


/*
 * Test RTC is readable.
 *
 * myAssert() will reset the mcu.
 * Although that probably will not alleviate the fatal condition, since it doesn't reset the RTC.
 *
 * Note also that design errors could cause this to never return: see isReadable()
 */
void Alarm::verifyRTCReadable() {
    // TODO needed?
    ///Test::delayBriefly();
    ///__delay_cycles(10000);

    // assert can talk to RTC
    myAssert(RTC::isReadable());

    // An older design checks the FOUT/nIRQ pin to test readiness of RTC
    // OLD Alarm::resetIfRTCNotReady();
}



/*
 * Configuration after coldstart.
 *
 * Not assume RTC was reset also.
 * RTC might not have powered down and up.
 * RTC could be in some bizarre state.
 */
void Alarm::configureAfterColdReset() {
    // GPIO must be unlocked because we configure bus pins and use them
    myRequire(not PMM::isLockedLPM5());

    /*
     * Configure:
     * - bus interface and driver (e.g I2C and eUSCI_B)
     * - alarm pin
     */
    Alarm::configureMcuSide();

    verifyRTCReadable();

    Alarm::configureRTC();

    _isConfiguredForAlarming = true;
    // Ensure MCU bus interface and RTC chip are configured for alarming
}




void Alarm::configureAfterWake() {

    // Only need to configure bus; alarm pin is still configured.
    Alarm::configureMcuBusInterface();

    verifyRTCReadable();

    // No need to configure RTC chip itself; assume is still configured

    _isConfiguredForAlarming = true;
}



void Alarm::configureMcuSide() {
    // Two parts: alarm interrupt pins and bus pin

    // Must precede isRTCReady (which reads the alarm pin)
    // TODO this has already been done by duty cycle framework???
    Alarm::configureMcuAlarmInterface();

    // Must precede use of bus to configure rtc
    Alarm::configureMcuBusInterface();
}

// TODO this should be done somewhere else when there are two slaves
void Alarm::configureMcuBusInterface(){
    /*
     * TODO hardcoded.
     * This works for AB0815 SPI, false means RW bit is not high (is low) for a read
     * Also works for AB0805 I2C, which ignores the actual parameter.
     */
    Bridge::configureMcuSide(false);
}



void Alarm::unconfigureMcuBusInterface() {
    Bridge::unconfigureMcuSide();
    _isConfiguredForAlarming = false;
}


void Alarm::configureMcuAlarmInterface() {
    /*
     * Pin is high when no interrupt.
     * RTC pulses low on interrupt.
     * Pulse width is relatively long (1/4 second)
     * Use trailing edge, low-to-high
     */
    AlarmPin::configurePullupLoToHiInterrupt();
}

bool Alarm::isConfiguredMcuAlarmInterface() {
    return AlarmPin::isConfigured();
}


void Alarm::configureRTC() {
    // require RTC bus interface configured
    /*
     * Require alarm is behind the counter.
     * Otherwise, by chance it could soon match,
     * resulting in an alarm coming before calling setAlarm().
     */

    // Order of configuration not important.
    RTC::configure24HourMode();

    RTC::configureRCCalibratedOscillatorMode();

    RTC::configureAlarmInterruptToFoutnIRQPin();

    // Tell RTC chip to trigger interrupt on alarm register match
    RTC::enableAlarm();
}

