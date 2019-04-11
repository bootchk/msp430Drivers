#pragma once

/*
 * Partial enumeration of device's register addresses and other values.
 *
 * Only the ones we use.
 *
 * This should be machine-generated instead of hand coded.
 */

enum class RTCAddress : unsigned char {
//enum Address {
	Time =               0x00,
	Alarm =              0x08,
	Status =             0x0F,
	Control1 =           0x10,
	Control2 =           0x11,
	InterruptMask =      0x12,
	TimerControl =       0x18,
	OscillatorControl =  0x1c,
	ConfigurationKey =   0x1f,
	AutocalibrationFilter = 0x26,
	Identifier =         0x28

};


/*
 * Various magical values written to various registers.
 */
enum class Key : unsigned char {
//enum Key {
	UnlockOscillatorControl = 0xa1,     // unlocks OscillatorControl
	UnlockMiscRegisters      = 0x9d,	// unlocks certain other registers when written to ConfigurationKey
	AutocalibrationFilterEnable = 0xa0  // special value for AutocalibrationFilter
};
