/*
 *  Define attributes and pins of stepper driver chip and motor
 *
 *  !!! See also board.h.  There must be no conflicts.
 */


#include <gpio.h>


// Declare the motor
// Uncomment one
//#define MOTOR_SOYO_NIDEC 1
#define MOTOR_SYMBOL_TECH 1




// PPS is defined for full step pulses

#if MOTOR_SOYO_NIDEC

#define MOTOR_STEPS_PER_REV 200
// for 1.95k RPM @200 steps/rev
#define MOTOR_MAX_PPS       6500


#elif MOTOR_SYMBOL_TECH

#define MOTOR_STEPS_PER_REV 20

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
#define MOTOR_MAX_PPS       100

#else
    error
#endif




// Declare the driver chip and board
#define DRIVER_POLOLU_TI8834 1

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
#define STEPPER_MICROSTEP_SIZE_HALF 1


// Whether the driver chip's fault detect pin is connected to a GPIO
#define STEPPER_FAULT_DETECT_USED 0
// Whether the driver chip's sleep pin is connected to a GPIO
#define STEPPER_SLEEP_USED 0


// Declare pinout for DIR pin
#define STEPPER_DIR_PORT GPIO_PORT_P1
#define STEPPER_DIR_PIN  GPIO_PIN5

// Declare pinout for STEP pin
#define STEPPER_STEP_PORT GPIO_PORT_P1
#define STEPPER_STEP_PIN  GPIO_PIN4

#if STEPPER_SLEEP_USED
// Declare pinout for notSLEEP pin
#define STEPPER_NSLEEP_PORT GPIO_PORT_P1
#define STEPPER_NSLEEP_PIN  GPIO_PINX
#endif


// Declare pinout for pin that enables motor coil outputs
#define STEPPER_ENABLE_COILS_PORT GPIO_PORT_P1
#define STEPPER_ENABLE_COILS_PIN  GPIO_PIN6

#if STEPPER_FAULT_DETECT_USED
#define STEPPER_FAULT_PORT GPIO_PORT_P1
#define STEPPER_FAULT_PIN  GPIO_PIN7
#endif


#endif


