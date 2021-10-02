
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
//static const unsigned int MaxItersInDarkToDischargeLEDCapacitance = 200;
// FOR TESTING
static const unsigned int MaxItersInDarkToDischargeLEDCapacitance = 50000;

// Max duration of measuring light
//static const unsigned int MaxTicksInDarkToDischargeLEDCapacitance = 5000;   // 0.5 seconds
// 10000 is good for a Cree LED measuring visible light
// static const unsigned int MaxTicksInDarkToDischargeLEDCapacitance = 10000;   // 1 seconds
// Temporary: for measuring using a UV LED
static const unsigned int MaxTicksInDarkToDischargeLEDCapacitance = 65000;   // 6.5 seconds, nearly max int


// Experimentally determined ticks for LED capacitance to discharge in 10 lux.
// Typically about 6k ticks were measured.
static const unsigned int TicksInNightDarkToDischargeLEDCapacitance = 5000;


/*
 * Factor added to light sample to indicate dark.
 * Typically, in light discharge count is about 100.
 * Max iterations is 200.
 */
static const unsigned int DarkFactorLEDDischargeCount = 70;

/*
 * 50kbps seems to work with internal pullups of 30kohm and jumpers less than an inch.
 *
 * 400kbps is "fast mode".  Requires external pullups
 * It may seem to work, but often locks up at initialTransition() of the I2C driver.
 *
 * !!! DriverLib function that uses this parameter is not trustworthy, and actual bus speed is set elsewhere.
 */
//static const unsigned long I2CBusSpeed =  50000;
//static const unsigned long I2CBusSpeed = 100000;
static const unsigned long I2CBusSpeed = 250000;    // This corresponds to what is set elsewhere
//static const unsigned long I2CBusSpeed =   400000; // DriverLib will yield unsupported 500kbps if you use this


/*
 * maximum duration the application expects to alarm.
 * In seconds.
 * Code will assert if exceeded in call to setAlarm()
 */
// TEMP very low for accelerated testing
// static const unsigned long MaxPracticalAlarmDuration = 40;
// 24 hours.
static const unsigned long MaxPracticalAlarmDuration = 86400;
};



/*
 * Stepper:  how long to energize coil for single step
 */
// Units cycles
// At 1Mhz system clock:
// 10k is 10mSec
// 500 is .5mSec

static const unsigned int SingleStepDelay = 500;
