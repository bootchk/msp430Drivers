// Define attributes of stepper driver chip and motor


// Declare the motor
// Uncomment one
//#define MOTOR_SOYO_NIDEC 1
 #define MOTOR_SYMBOL_TECH 1

// Declare the driver chip and board
#define DRIVER_POLOLU_TI8834 1




#if MOTOR_SOYO_NIDEC

#define MOTOR_STEPS_PER_REV 200


#elif MOTOR_SYMBOL_TECH

#define MOTOR_STEPS_PER_REV 20

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
 * M0       : OLD
 * Enable   : OLD P1.6
 *
 * Config pin: unconnected, is internal high i.e. indexer mode
 * M1       : unconnected, is internal low
 */

// The board does not bring out the pins that allow StepSize to be chosen
#define STEPPER_STEP_SIZE_RUNTIME_CHOOSEABLE 0
// The board hardwires step size to a full detent step
#define STEPPER_HARD_STEP_SIZE_FULL 1


// Declare pinout for DIR pin
#define STEPPER_DIR_PORT GPIO_PORT_P1
#define STEPPER_DIR_PIN  GPIO_PIN3

// Declare pinout for STEP pin
#define STEPPER_STEP_PORT GPIO_PORT_P1
#define STEPPER_STEP_PIN  GPIO_PIN2


#endif


