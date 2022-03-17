/*
 *  Define attributes and pins of stepper driver chip and motor
 *
 *  !!! See also board.h.  There must be no conflicts.
 */


#include <gpio.h>


/*
 * MOTOR_MAX_PPS is defined for full step pulses.
 * More PPS are generated if microstepping is used.
 *
 * The max PPS of the driver chip is 250k.
 *
 * The max PPS of the motor sometimes is defined to imply the highest RPM
 * the motor will turn at some torque (torque usually lower at high RPM.)
 * Typically the RPM of a stepper is no more than 1200 or 20 RPS.
 * For a 200 step motor, that implies a max PPS of 4k is typical.
 *
 * Here MOTOR_MAX_PPS is defined differently:
 * It is the PPS (implying an RPM) that will start the motor turning under some load torque
 * (which must be less than the max starting torque from the motor datasheet.)
 */


// Declare the motor
// Uncomment one
//#define MOTOR_SOYO_NIDEC 1
#define MOTOR_SYMBOL_TECH 1


// Declare the driver chip and board
#define DRIVER_POLOLU_TI8834 1


// Overall configuration

// Whether the driver chip's fault detect pin is connected to a GPIO
#define STEPPER_FAULT_DETECT_USED 0

/*
 * Whether the driver chip's sleep pin is connected to a GPIO.
 * It can be hardwired to a logic signal.
 * It cannot be hardwired to the Vint pin (a high logic) of the DRV8834 chip.
 */
#define STEPPER_SLEEP_USED 1



#if MOTOR_SOYO_NIDEC

#define MOTOR_STEPS_PER_REV 200
#define DEGREES_PER_STEP    1.8

/*
 * The datasheet shows a torque to PPS curve with a highest PPS of 6500PPS for full step.
 * But the datasheet is suspect.
 *
 * Experiments show that the max PPS (for full detent steps) that will reliably turn the motor
 * is
 */
#define MOTOR_MAX_PPS_FULL_STEP       600


#elif MOTOR_SYMBOL_TECH

#define MOTOR_STEPS_PER_REV 20
#define DEGREES_PER_STEP    18

/*
 * Max PPS and RPM not known from datasheet which is not available
 *
 * for 2k RPM @20 steps/rev is 40k steps/minute, or 666 steps/second.
 *
 * for 1.2k RPM @20 step/rev is 24k steps/minute, or 400 steps/second
 *
 * for 300 RPM @20 steps/rev is 6k steps/min or 100 steps/second
 *
 * for 200 RPM @20 steps/rev is 4k steps/min or 67 steps/second
 *
 * By experiment, 500 PPS loses track and 250PPS does not, for full step microstepping
 */
#define MOTOR_MAX_PPS_FULL_STEP       100

#else
    error
#endif






#ifdef DRIVER_POLOLU_TI8834

/*
 * Connections:
 * MSP430 to TI DRV8834 stepper motor driver
 *
 * Chip Pin : MSP430pin
 * --------------------
 * Step     : P1.2
 * Dir      : P1.3
 *
 *
 * Not sleep: OLD P1.4
 * Enable   : OLD P1.6
 *
 * Config pin: unconnected, is internal high i.e. indexer mode
 * M1       : unconnected, is internal low
 * M0       : jumpered to GND, full step
 */

#define STEPPER_STEP_SIZE_RUNTIME_CHOOSEABLE 0

/*
 * The board does bring out the pins that allow StepSize to be chosen at runtime,
 * but we choose not to connect to GPIO pins, but to jumper them i.e. hardwired.
 *
 * For the default M1 (not connected, internally pulled down) :
 * If M0 pin is tied to logic low (ground), is full step.
 * If M0 pin is tied to logic high (mcu vcc 3.3V), is half step.
 * If M0 pin floats, is quarter step.
 *
 * Uncomment one to match the board and jumpers.
 */


// The board hardwires step size to a full detent step, pin M0 is grounded
//#define STEPPER_MICROSTEP_SIZE_FULL 1
// The board hardwires step size to a half detent step, pin M0 is high
//#define STEPPER_MICROSTEP_SIZE_HALF 1
// The board hardwires step size to a quarter detent step, pin M0 is floating
#define STEPPER_MICROSTEP_SIZE_QUARTER 1

/*
 * Quarter stepping is best for SOYO NEMA motor.
 * At full stepping and MAX_PPS of 600, get erratic motion at slow commanded speed.
 */




// pinout for DIR pin
#define STEPPER_DIR_PORT GPIO_PORT_P1
#define STEPPER_DIR_PIN  GPIO_PIN5

// pinout for STEP pin
#define STEPPER_STEP_PORT GPIO_PORT_P1
#define STEPPER_STEP_PIN  GPIO_PIN4



#if STEPPER_SLEEP_USED
// Declare pinout for notSLEEP pin
#define STEPPER_NSLEEP_PORT GPIO_PORT_P1
#define STEPPER_NSLEEP_PIN  GPIO_PIN7
#endif


// pinout for Enable pin, that enables motor coil outputs
#define STEPPER_ENABLE_COILS_PORT GPIO_PORT_P1
#define STEPPER_ENABLE_COILS_PIN  GPIO_PIN6

#if STEPPER_FAULT_DETECT_USED
#define STEPPER_FAULT_PORT GPIO_PORT_P1
#define STEPPER_FAULT_PIN  GPIO_PIN7
#endif

// pinout for CONFIG pin, sets mode to Indexer or Phase/Enable
#define STEPPER_CONFIG_MODE_PORT GPIO_PORT_P2
#define STEPPER_CONFIG_MODE_PIN  GPIO_PIN2



#endif  // DRV8834


