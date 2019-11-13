
#include "realTimeClock.h"

#include "AB08xxRegisters.h"

#include "RTCInterface.h"

#include "../../bridge/bridge.h"

#include "../../assert/myAssert.h"





/*
 * Configuration part of RTC
 */


/*
 The chain from alarm on RTC to interrupt on MCU:
 - RTC alarm match is enabled
 - RTC alarm register matching time registers sets Status.ALM bit
 - RTC ALM controls the AIRQ signal
 - RTC AIRQ signal is configured to pulse down the pin Fout/nIRQ.
 - MCU GPIO pin for alarm is configured to interrupt on rising edge
 - MCU alarm pin interrupt flag is set
 - ISR called (since interrupt enabled)
 */


/*
 * 24 hour mode is an implementation decision that affects math operations.
 * Match once per year is an implentation decision that affects writing alarm registers.
 */
bool RTC::configureStandardAlarming() {
    return ( RTCInterface::configureAlarmMatchPerYear()
    and RTCInterface::configure24HourMode() );
}




void RTC::configureRCCalibratedOscillatorMode() {
	// Set two separate registers
	RTC::selectOscModeRCCalibratedWithAutocalibrationPeriod();
	RTC::enableAutocalibrationFilter();
}



bool  RTC::configureAlarmInterruptToFoutnIRQPin() {
	// OLD :RTC::enablePulseInterruptForAlarm();
    // NEW : we don't enable until setAlarm()
	return RTC::connectFoutnIRQPinToAlarmSignal();
}








bool RTC::is24HourModeConfigured() {
    return ( isRegisterHaveBitsClear(RTCAddress::Control1, 0b01000000) );
}






/*
 * Private
 */


void RTC::selectOscModeRCCalibratedWithAutocalibrationPeriod() {

	unlockOscControlRegister();

	/*
	 * ??? Fails to verify often, thus we do not verify (writeByteWriteOnly() instead of writeByte())
	 */

	// OSEL on => BIT7;
	// ACAL == 10 (17 minute autocalibration period) => BIT6
	//Bridge::writeByteWriteOnly(static_cast<unsigned char>(RTCAddress::OscillatorControl), (unsigned char) 0b11000000 ); // (BIT7 | BIT6) );
    Bridge::writeByte(static_cast<unsigned char>(RTCAddress::OscillatorControl), (unsigned char) 0b11000000 ); // (BIT7 | BIT6) );
}


void RTC::enableAutocalibrationFilter() {
	unlockMiscRegisters();

	// Enable filter
	Bridge::writeByte(static_cast<unsigned char>(RTCAddress::AutocalibrationFilter),
	              (unsigned char) Key::AutocalibrationFilterEnable );
}




/*
 * Related to alarm and its interrupt.
 */


/*
 * Only bit 2, not the other bits that configure the pulse
 */
bool RTC::isAlarmInterruptEnabled(){
    return isRegisterHaveBitsSet(RTCAddress::InterruptMask, 0b100 );
}

bool RTC::isAlarmInterruptConfiguredPulse(){
    return isRegisterHaveBitsSet(RTCAddress::InterruptMask, 0b01100000 );
}




bool RTC::connectFoutnIRQPinToAlarmSignal() {
	/*
	 * Connects signals to pin.
	 * Here, we connect only the rtc's internal nAIRQ signal (from alarm)
	 * bits 0,1: OUT1S: == 11, pin is signal nAIRQ if AIE is set, else OUT
	 * I.E. OUT is always 1, so signal on pin is high, but dips low when signal nAIRQ  dips
	 */
    return Bridge::writeByte(static_cast<unsigned char>(RTCAddress::Control2),  0b11 );
}

bool RTC::isAlarmConfiguredToFoutnIRQPin(){ return isRegisterHaveBitsSet(RTCAddress::Control2, 0b11); }







bool RTC::isAlarmConfiguredMatchPerYear(){
    return (isRegisterHaveBitsSet(RTCAddress::TimerControl, 0b100)
            and isRegisterHaveBitsClear(RTCAddress::TimerControl, 0b11000));
}
// TODO why was 11100 ???




#ifdef OLD
This is NOT a good way to disable alarm.

void RTC::disableAlarm() {
    Bridge::writeByte(static_cast<unsigned char>(RTCAddress::TimerControl), 0 );
}
#endif








/*
 * Unlocking.
 * !!! Writes to ConfigurationKey cannot be verified, since it resets to zero upon any write.
 * It is really a lock: writing the key unlocks other registers.
 * The write doesn't change the value in the register (it always reads zero.)
 */
void RTC::unlockMiscRegisters() {
	Bridge::writeByteWriteOnly(static_cast<unsigned char>(RTCAddress::ConfigurationKey),
	                           (unsigned char) Key::UnlockMiscRegisters );
}


void RTC::unlockOscControlRegister() {
	Bridge::writeByteWriteOnly(static_cast<unsigned char>(RTCAddress::ConfigurationKey),
	                           (unsigned char) Key::UnlockOscillatorControl );
}





/*
 * Whole chain of configuration re alarm.
 *
 * Starts with ALM bits set so that match sets a flag.
 * Followed by configuration so that flag setting causes interrupt pulse on a pin.
 *
 * These do NOT check that alarm registers have a value that is in the future.
 */
bool RTC::isAlarmInterruptConfigured() {
    return (isAlarmFlaggingConfigured()
            and isAlarmInterruptEnabled()
            and isAlarmInterruptConfiguredPulse()
            and isAlarmConfiguredToFoutnIRQPin()
            and isAlarmFlagClear()
            );

}


bool RTC::isAlarmFlaggingConfigured() {
    // FUTURE also must be 24 hour mode or alarm match might not work
    return ( isAlarmConfiguredMatchPerYear()
            and is24HourModeConfigured());
}
