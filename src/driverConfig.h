
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
 * Whether to verify by reading back everything that was written across Bridge.
 */
//#define VERIFY_BRIDGE_WRITES
