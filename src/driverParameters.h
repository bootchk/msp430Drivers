
#pragma once

/*
 * Constant parameters of driver
 */

class DriverConstant {

public:
#ifdef OLD
 /*
  * NEW design:
  *  Now if loop never break (iterations >= max, then is dark.
  *  I.E. We don't need two parameters, only one.
  */
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
static const unsigned int MinItersInLightToDischargeLEDCapacitance = 100;
#endif

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
static const unsigned int MaxItersInDarkToDischargeLEDCapacitance = 200;
// FOR TESTING static const unsigned int MaxItersInDarkToDischargeLEDCapacitance = 1000;


/*
 * 50kbps seems to work with internal pullups of 30kohm and jumpers less than an inch.
 *
 * 400kbps is "fast mode".  Requires external pullups
 */
//static const unsigned long I2CBusSpeed = 50000;
//static const unsigned long I2CBusSpeed = 100000;
static const unsigned long I2CBusSpeed = 400000;


/*
 * maximum duration the application expects to alarm.
 * In seconds.
 */
// TEMP very low for accelerated testing
static const unsigned long MaxPracticalAlarmDuration = 40;
};



/*
 * Stepper:  how long to energize coil for single step
 */
// Units cycles
// At 1Mhz system clock:
// 10k is 10mSec
// 500 is .5mSec

static const unsigned int SingleStepDelay = 500;
