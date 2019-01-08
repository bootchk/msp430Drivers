
#pragma once

/*
 * Constant parameters of driver
 */

class DriverConstant {

public:
/*
 * Count of loop cycles discharging reversed biased LED to sense light.
 *
 * If an external strong light illuminates the LED,
 * the LED as a solar cell will discharge the charged capacitance of the LED in fewer loop iterations.
 */
/*
 * Typical loop iterations when LED is in strong light.
 * Depends on the LED.
 */
static const unsigned int MinCyclesInLightToDischargeLEDCapacitance = 90;

/*
 * Maximum loop iterations we will attempt to discharge LED capacitance.
 * Typical loop iterations when LED is in the dark is greater than this.
 * From experimentations, say 1000 loop iterations at 100 lux (dimly lit room)
 * Usually a dark LED will discharge in about this count of cycles.
 * This only insures that the loop takes a small finite time.
 *
 * See the code for sensing light.
 * We count up.  This value should be only about ten percent higher than the above value.
 */
static const unsigned int MaxCyclesInDarkToDischargeLEDCapacitance = 100;
};
