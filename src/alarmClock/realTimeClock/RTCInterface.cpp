
#include "RTCInterface.h"

#include "AB08xxRegisters.h"

#include "../../bridge/bridge.h"



/*
 * Does not check for oscillator faults (unlike Linux driver.)
 * On the AB08x5, the clock runs despite oscillator faults.
 * An oscillator fault only means that the time may not be correct in relation to an external reference clock (slower or faster.)
 */



/*
 * RTCTime is an amalgam.
 * It contains YearOfCentury field.
 * If all of RTCtime is written to alarm registers, it writes the YearOfCentury field into the alarm "Weekday" register.
 * That would will read back correctly.
 * That would work also, since the configured RTC alarm mode is ignoring the "Weekday" register.
 *
 * But here we don't write RTCTime.year into the alarm registers.
 */

bool RTCInterface::writeAlarm(const RTCTime* alarm) {
    return Bridge::write(static_cast<unsigned char> (RTCAddress::Alarm),
                 (unsigned char *) alarm,
                 COUNT_BYTES_READ_WRITE_TO_ALARM);  // OLD sizeof(RTCTime));
    // assert alarm parameter is unchanged.
    // assert alarm was written to RTC
}


bool RTCInterface::readAlarm(RTCTime* alarm) {
    return Bridge::read(static_cast<unsigned char> (RTCAddress::Alarm),
                       (unsigned char *) alarm,
                       COUNT_BYTES_READ_WRITE_TO_ALARM);
}


bool RTCInterface::setAlarm(const RTCTime* alarm) {
    if (writeAlarm(alarm)) {
        return enableAlarmIRQ();
    }
    else return false;
}

bool RTCInterface::enableAlarmIRQ() {
    /*
     * Bit 2:  AIE: enable alarm interrupt
     * Bit 5,6: IM: == 11 :  1/4 second pulse width, requires least power
     */
    return Bridge::writeByte(static_cast<unsigned char>(RTCAddress::InterruptMask),   0b01100100 );

    // Polarity of interrupt is not configurable on RTC, occurs on high-to-low edge
}


bool RTCInterface::readTime(RTCTime* time) {
    return Bridge::read(static_cast<unsigned char> (RTCAddress::Time),
                       (unsigned char *) time,
                       sizeof(RTCTime));
}


bool RTCInterface::configureAlarmMatchPerYear() {
    /*
     * Reset state of TimerControl is 0b100011 ???.
     *
     * Bits [4:2]==001 => match once per year
     * Bits [4:2]==000 => match disabled
     */
    return Bridge::writeByte(static_cast<unsigned char>(RTCAddress::TimerControl), 0b100 );
}


/*
 * We assume the STOP(0), ARST (0), OUT (1), and WRTIC(1) bits remain at their reset values.
 * STOP  BIT7 == 0 -> not stopped
 * 12/24 BIT6 == 0 -> 24 hour mode
 * OUT   BIT5 == 1  level driven on FOUT/nIRQ
 * ARST  BIT2 == 0  no auto reset on reading STATUS
 * WRTC  BIT0 == 0 -> counter registers locked
 *
 * !!! Note that bit 5 is not writeable when LKO2 bits is set.
 *
 * Like the Linux driver, we read the register and modify bits.
 * We must leave the OUT bit alone, since it drives the FOUT/nIRQ pin usually, and we want to leave it set.
 * Unlike the Linux driver, we do not:
 * - set the WRTC bit (allowing time registers to be written)
 * - clear the ARST bit (means that a read of STATUS clears it).
 */
bool RTCInterface::configure24HourMode() {
    return Bridge::clearBits(static_cast<unsigned char>(RTCAddress::Control1),  (unsigned char) 0b01000000 ); // BIT6
}

/*
     * Configure hour counter to count to 24.
     * !!! If you omit, upper nibble of hour counter is NOT hours tens digit, but include bit for AM/PM
     */
