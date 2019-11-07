
#include "realTimeClock.h"

#include "AB08xxRegisters.h"

#include "../../bridge/bridge.h"

#include "../../assert/myAssert.h"





/*
 * Implementation knows:
 * - RTC is connected via SPI
 * - which register and bit, semantics of same
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



void RTC::clearAlarmFlag() {
	/*
	 * Since we are only using one interrupt from RTC,
	 * it is safe to clear them all by writing a zero to the flag byte.
	 * Per AB08x5 User Manual section 5.12.9
	 *
	 * Register name is Status
	 * Bit name is ALM, but we clear all bits
	 */

	Bridge::writeByte(static_cast<unsigned char>(RTCAddress::Status),
	              0);
	myEnsure(isAlarmFlagClear());
}

bool RTC::isAlarmFlagClear(){
    unsigned char value = Bridge::readByte(static_cast<unsigned char>(RTCAddress::Status));
    // BIT2 is clear
    return ((value & 0b0100) == 0);
}



void RTC::configureRCCalibratedOscillatorMode() {
    // TEMP
    // TODO
    // This is optional, and is failing to verify writes.
    return;

	// Set two separate registers
	RTC::selectOscModeRCCalibratedWithAutocalibrationPeriod();
	RTC::enableAutocalibrationFilter();
}



void RTC::configureAlarmInterruptToFoutnIRQPin() {
	// Set two separate registers

	// Order not important
	RTC::enablePulseInterruptForAlarm();
	RTC::connectFoutnIRQPinToAlarmSignal();
}





/*
 * BIT7 == 0 -> not stopped
 * BIT6 == 0 -> 24 hour mode
 * BIT0 == 0 -> counter registers locked
 *
 * !!! Note that bit 5 is not writeable when LKO2 bits is set.
 * Must use writeBits: does not ensure that given mask is the final contents of register.
 */
void RTC::configure24HourMode() {
	Bridge::clearBits(static_cast<unsigned char>(RTCAddress::Control1),
	                  (unsigned char) 0b01000000 ); // BIT6
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
	Bridge::writeByteWriteOnly(static_cast<unsigned char>(RTCAddress::OscillatorControl),
	              (unsigned char) 0b11000000 ); // (BIT7 | BIT6) );
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

void RTC::enablePulseInterruptForAlarm() {
	/*
	 * Bit 2:  AIE: enable alarm interrupt
	 * Bit 5,6: IM: == 11 :  1/4 second pulse width, requires least power
	 */
	Bridge::writeByte(static_cast<unsigned char>(RTCAddress::InterruptMask),
	              0b01100100 );

	// Polarity of interrupt is not configurable on RTC, occurs on high-to-low edge

	myEnsure(RTC::isAlarmInterruptEnabled());
}

/*
 * Only bit 2, not the other bits that configure the pulse
 */
bool RTC::isAlarmInterruptEnabled(){
    unsigned char value = Bridge::readByte(static_cast<unsigned char>(RTCAddress::InterruptMask));
    return (value & 0b100);
}

bool RTC::isAlarmInterruptConfiguredPulse(){
    unsigned char value = Bridge::readByte(static_cast<unsigned char>(RTCAddress::InterruptMask));
    return (value & 0b01100000);
}




void RTC::connectFoutnIRQPinToAlarmSignal() {
	/*
	 * Connects signals to pin.
	 * Here, we connect only the rtc's internal nAIRQ signal (from alarm)
	 * bits 0,1: OUT1S: == 11, pin is signal nAIRQ if AIE is set, else OUT
	 */
	Bridge::writeByte(static_cast<unsigned char>(RTCAddress::Control2),
	              0b11 );
}

bool RTC::isAlarmConfiguredToFoutnIRQPin(){
    unsigned char value = Bridge::readByte(static_cast<unsigned char>(RTCAddress::Control2));
    return (value & 0b11);
}


void RTC::configureAlarmMatchPerYear() {
    /*
     * Reset state of TimerControl is 0b100011 ???.
     *
     * Bits [4:2]==1 => match once per year
     * Bits [4:2]==0 => match disabled
     */
    Bridge::writeByte(static_cast<unsigned char>(RTCAddress::TimerControl), 0b100 );
    myEnsure(isAlarmFlaggingConfigured());
}

void RTC::disableAlarm() {
    Bridge::writeByte(static_cast<unsigned char>(RTCAddress::TimerControl), 0 );
}



bool RTC::isAlarmConfiguredMatchPerYear(){
    bool result;

    unsigned char value = Bridge::readByte(static_cast<unsigned char>(RTCAddress::TimerControl));
    // TODO why was 11100 ???
    result = (value & 0b100);
    // TEMP
    myAssert(result);
    return result;
}




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






bool RTC::isReadable() {
    /*
     * Sane if upper byte of part ID reads as 08, from 08xx part family.
     *
     * Read will return 0 if RTC chip is non-responsive (HW failure in RTC chip.)
     * The Master is clocking.  Slave non-responsive means failing to drive data line with ID value.
     * For SPI bus, this seems to work.
     * For I2C bus, the Slave may Nack a written byte and that asserts in the I2C state machine.
     *
     * For I2C bus, if the Slave stretches the clock, this does not use the timeout capability of the driver, and thus may hang.
     *
     * Not require RTC chip itself configured.
     * Require bus interface configured.
     *
     * Note there is no redundant timing based check here.
     * If the bus hangs, this may never return.
     * The bus could hang through faulty SW design
     * or failure to account for timing using delays.
     *
     * If it does hang, it should consume power and eventually brownout the system
     * and then the system (mcu and RTC chip) reset when power is restored.
     *
     * This costs a bus transfer.
     * It is optional: if omitted, later RTC operations should fail anyway.
     */
    unsigned char ID;

    ID = Bridge::readByte(static_cast<unsigned char>(RTCAddress::Identifier));
    return (ID == 0x08);
}


bool RTC::readOUTBit() {
    unsigned char control1 = Bridge::readByte(static_cast<unsigned char>(RTCAddress::Control1));
    return (control1 & 0b10000);    // BIT4
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
    return isAlarmConfiguredMatchPerYear();
}
