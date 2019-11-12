
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





bool RTC::clearAlarmFlag() {
	/*
	 * Since we are only using one interrupt from RTC,
	 * it is safe to clear them all by writing a zero to the flag byte.
	 * Per AB08x5 User Manual section 5.12.9
	 *
	 * Register name is Status
	 * Bit name is ALM, but we clear all bits
	 */

	bool result;
	result = Bridge::writeByte(static_cast<unsigned char>(RTCAddress::Status),
	              0);
	//myEnsure(isAlarmFlagClear());
	return result;
}

bool RTC::isAlarmFlagClear(){

    unsigned char value;
    if (not Bridge::readByte(static_cast<unsigned char>(RTCAddress::Status), &value) ) {
        return false;
    }
    else {
        // BIT2 is clear
       return ((value & 0b0100) == 0);
    }
}







bool RTC::isRegisterHaveBitsSet(RTCAddress registerAddress, unsigned char bitMask) {
    unsigned char actualValue;
   if ( not Bridge::readByte(static_cast<unsigned char>(registerAddress), &actualValue) ) {
        return false;
   }
   else {
        return (bitMask & actualValue);
   }
}

bool RTC::isRegisterHaveBitsClear(RTCAddress registerAddress, unsigned char bitMask) {
    unsigned char actualValue;
   if ( not Bridge::readByte(static_cast<unsigned char>(registerAddress), &actualValue) ) {
        return false;
   }
   else {
        return ((bitMask & actualValue) == 0);
   }
}

bool RTC::isRegisterHaveValue(RTCAddress registerAddress, unsigned char desiredValue) {
    unsigned char actualValue;
   if ( not Bridge::readByte(static_cast<unsigned char>(registerAddress), &actualValue) ) {
        return false;
   }
   else {
        return (desiredValue == actualValue);
   }
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
    return isRegisterHaveValue(RTCAddress::Identifier, 0x08);
}


bool RTC::isOUTBitSet() {
    return isRegisterHaveBitsSet(RTCAddress::Control1, 0b10000);    // BIT4
}


