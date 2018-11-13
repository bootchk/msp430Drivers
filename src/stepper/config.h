


/*
 * Compile time definition of mcu pins connected to driver IN pins
 * For example, to pins 3,4,5,6 of MSP430F2012 connected to pins AIN1, AIN2, BIN1, BIN2 of DRV8835
 *
 * BIT0 is usually an LED on a launchpad target board
 */

#define MotorA1 BIT1    // Coil A
#define MotorA2 BIT2
#define MotorB1 BIT3    // Coil B
#define MotorB2 BIT4

// to gate of mosfet power switch that switches Vcc to driver
#define DrivePowerSwitch BIT5
