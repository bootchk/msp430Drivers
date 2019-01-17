


/*
 * Compile time definition of mcu pins connected to driver IN pins
 * For example, to pins 3,4,5,6 of MSP430F2012 connected to pins AIN1, AIN2, BIN1, BIN2 of DRV8835
 *
 * BIT0 is usually an LED on a launchpad target board
 */

#define MSPEXP4233  // my breadboard, 4 consecutive pins of Launchpad
#ifdef MSPEXP4233
#define MotorA1 BIT0    // Coil A
#define MotorA2 BIT5
#define MotorB1 BIT4    // Coil B
#define MotorB2 BIT6

// to gate of mosfet power switch that switches Vcc to driver
#define DrivePowerSwitch BIT7
#endif

// Units cycles
// At 1Mhz system clock:
// 10k is 10mSec
// 500 is .5mSec
#define SingleStepDelay 500 //10000
