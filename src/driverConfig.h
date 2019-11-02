
/*
 * Configures the msp430Drivers code
 * (for aspects other than GPIO configuration of the PCB)
 */




/*
 * Configure actions for fatal conditions.
 */

// On production PCB: action is reboot
//#define FATAL_PRODUCTION 1

// For testing with Launchpad prototype: action is warble red or green LED forever
//#define FATAL_TESTING 1

// For testing with Launchpad prototype: action is the halt (infinite loop) defined by C assert()
// When you pause the running program, you will be in assert.c defined by the standard C runtime
#define FATAL_DEBUGGING 1




/*
 * Serial bus implementation is either I2C OR SPI, but not both
 */
#define SERIAL_IS_I2C
//#define SERIAL_IS_SPI

/*
 * Whether I2C has external pullups is property in board.h
 */


/*
 * Whether to verify single byte writes by reading back and comparing to what was written.
 */
#define VERIFY_BRIDGE_SINGLE_WRITES






/*
 * LowPowerTimer use RTC or WDT
 */
#define LOW_POWER_TIMER_USE_RTC 1
//#define LOW_POWER_TIMER_USE_WDT 1



/*
 * LEDAndLightSensor implementation
 */
//#define LIGHT_SENSE_ITERATIVE
#define LIGHT_SENSE_SLEEPING
