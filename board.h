
#pragma once


/*
board.h

Configures mspDrivers library.
Is an abstraction layer.
Defines virtual devices in terms of actual devices on MCU
and in terms of a PCB board.

This example is for the test harness of msp430Drivers.
NOT in /src and hence not part of the library.
A real app would define board.h higher in the dependency tree.

The CCS IDE defines macros for the target mcu
e.g. __MSP430FR2433__
mspDrivers lib modules should not reference those definitions.
Only this file should.

When msp430Drivers throws errors,
ensure that board.h was included.
*/


#pragma once


// Some right hand sides from gpio.h in DriverLib
#include <gpio.h>

// Depends on configuration of app
#include "src/config/driverConfig.h"
#include "src/config/busConfig.h"


/*
DriverLib defines like __MSP430_HAS_CS__
But here we augment.
*/
#ifdef __MSP430FR2433__
    #define USE_PORT3 1
    #define disableLowXT CS_turnOffXT1
    // Declare timer peripheral to use
    #define TIMER_BASE_ADDRESS TIMER_A0_BASE
#endif
#ifdef __MSP430FR2311__
    #define USE_PORT3 0
    #define disableLowXT CS_turnOffXT1
    #define TIMER_BASE_ADDRESS TIMER_B0_BASE
    // Redirect Timer_A calls and constants
    // Assumes that TimerB really is similar to TimerA
    // i.e. DriverLib code is same for A and B
    #define Timer_A_stop      Timer_B_stop
    #define Timer_A_outputPWM Timer_B_outputPWM
    #define Timer_A_outputPWMParam Timer_B_outputPWMParam
    #define TIMER_A_CLOCKSOURCE_ACLK TIMER_B_CLOCKSOURCE_ACLK
    #define TIMER_A_CLOCKSOURCE_DIVIDER_1 TIMER_B_CLOCKSOURCE_DIVIDER_1
    #define TIMER_A_CAPTURECOMPARE_REGISTER_1 TIMER_B_CAPTURECOMPARE_REGISTER_1
    #define TIMER_A_OUTPUTMODE_RESET_SET TIMER_B_OUTPUTMODE_RESET_SET
#endif

/*
 * Describes hardware design i.e. connections between pins of mcu and rtc chips
 *
 * Local alias to definitions specific to:
 * - choice of family member (here MSP430FR2433)
 * - choice of instance of serial device
 * - choice of alarm pin
 * - choice of LED source/sunk or double ended
 */


/*
Define the board instance.

Choose exactly one board.
Board may be a breadboard of Launchpad's target and sub-boards
*/
//#define MYPCB_BOARD_R5
#define FULL_PROTO_LAUNCHPAD
//#define BLINKERBQ_BOARD


// Choose exactly one Launchpad as board
// Only required when the board uses target board of launchpad
#define USE_EXP430FR2433
//#define USE_EXP430FRFR698


// Depends on board instance defined already
#include "src/config/ledAndLightSensorConfig.h"


/*
 * Whether I2C bus has external pullups
 */
#ifdef FULL_PROTO_LAUNCHPAD
#define I2C_HAS_EXTERNAL_PULLUPS
#elif defined(MYPCB_BOARD_R5)
#define I2C_HAS_INTERNAL_PULLUPS
#else
#error "board.h does not define I2C pullups"
#endif


#ifdef USE_EXP430FR2433

#define I2C_ON_P12_P13
#define APP_LED_ON_P10_P11


#elif defined(USE_EXP430FRFR698)

// FR6989
// Choose interface to external RTC I2C
#define I2C_ON_P16_P17

// TODO move this to ledAndLightSensorConfig.h
// Choose interface to LED and light sensor
#define LED_SENSOR_ON_P13_P15

// Choose debug LED config
#define APP_LED_ON_P10_P97


#else
#error "Launchpad model not defined."
#endif


/*
 * !!! If you change this, be sure to also change App::configureUnusedPins()
 */



/*
 * You must define
 * - SPI: MOSI, MISO, CLCK
 * - Alarm pin
 * - SPI slave select pin
 *
 * LEDAndLightSensor configuration
 *
 * For rev 1 board hack #1, LEDP 1.0, LEDN 1.4
 * For rev 1 board hack #2, LEDP 1.1, LEDN 1.0 (P1.1 was LEDpwm)
 *
 * Two GPIO pins, one for each side of LED.
 * Must not conflict with other uses.
 */


/*
 * Address of chosen device instance on chosen family member.
 * MSP430FR2433 choices are A0, A1, B0
 * !!! if family member does not contain the choice of instance, you get compile errors.
 */

// Unused config: msp430fr2433 specific, using instance UCB0: 1.2, 1.3, 1.1


#ifdef PROTO_BOARD

#define USE_EUSCI_A0
#define SPIInstanceAddress      EUSCI_A0_BASE

// msp430fr2433 specific, using instance UCA0

// P1.4
#define MOSI_PORT     GPIO_PORT_P1
#define MOSI_PIN      GPIO_PIN4
// P1.5
#define MISO_PORT     GPIO_PORT_P1
#define MISO_PIN      GPIO_PIN5
// P1.6
#define SPI_CLK_PORT  GPIO_PORT_P1
#define SPI_CLK_PIN   GPIO_PIN6


// P2.7
#define AlarmSignalPort GPIO_PORT_P2
#define AlarmSignalPin  GPIO_PIN7

// P1.7 UCA0STE doesn't work?
// P2.0 (must not conflict with Xin)
// p3.2
#define RTCSelectPort GPIO_PORT_P3
#define RTCSelectPin  GPIO_PIN2

#endif



#ifdef MYPCB_BOARD_R3

// Configuration of PCB
// And also configuration of one prototype on launchpad




#ifdef SERIAL_IS_SPI
#define SPIInstanceAddress      EUSCI_A1_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCA1:
// MOSI 2.6
// MISO 2.5
// CLK 2.4
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6

#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5

#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4

// P2.2
#define RTCSelectPort GPIO_PORT_P2
#define RTCSelectPin  GPIO_PIN2

#endif


#endif  // MYPCB_BOARD_R3




// !!! slight difference from PCB: alarm pin not 3.0, chip select not 2.2

#ifdef SERIAL_IS_SPI
#define SPIInstanceAddress      EUSCI_A1_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCA1:
// MOSI 2.6
// MISO 2.5
// CLK 2.4
#define MOSI_PORT     GPIO_PORT_P2
#define MOSI_PIN      GPIO_PIN6

#define MISO_PORT     GPIO_PORT_P2
#define MISO_PIN      GPIO_PIN5

#define SPI_CLK_PORT  GPIO_PORT_P2
#define SPI_CLK_PIN   GPIO_PIN4


#ifdef SPI_ALARM_PIN_1_3
// P1.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN3
#else
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN4
#endif

// P1.2
#define RTCSelectPort GPIO_PORT_P1
#define RTCSelectPin  GPIO_PIN2


#endif




#ifdef I2C_ON_P12_P13
/*
 * Unused.  For compass using I2C
 */
#define I2CInstanceAddress      EUSCI_B0_BASE

// msp430fr2433 and Launchpad MSP-EXP430FR2433, using instance UCB0:
// data P1.2
// clock P1.3
#define I2C_SDA_PORT  GPIO_PORT_P1
#define I2C_SDA_PIN   GPIO_PIN2

#define I2C_SCL_PORT  GPIO_PORT_P1
#define I2C_SCL_PIN   GPIO_PIN3

// In various revision of my board, was P2.3
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN1

#endif



#ifdef I2C_ON_P16_P17

#define I2CInstanceAddress      EUSCI_B0_BASE

#define I2C_SDA_PORT  GPIO_PORT_P1
#define I2C_SDA_PIN   GPIO_PIN6

#define I2C_SCL_PORT  GPIO_PORT_P1
#define I2C_SCL_PIN   GPIO_PIN7


#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN4

#endif




/*
 * LED configuration
 */

/*
 * Definitions for MSP-EXP430FR2433: two LED's
 *
 * !!! These are only for class LED, where LED is driven from a single side.
 * See also LEDAndLightSensor class
 */


// On PCB, LED is sunk
// TODO for prototype on launchpad, led is sourced
#define LED_SOURCED

#ifdef APP_LED_ON_P10_P97

// red
#define APP_LED1_PORT     GPIO_PORT_P1
#define APP_LED1_PIN      GPIO_PIN0

// green
#define APP_LED2_PORT     GPIO_PORT_P9
#define APP_LED2_PIN      GPIO_PIN7

#endif

#ifdef APP_LED_ON_P10_P11

// launchpad FR2433
// red
#define APP_LED1_PORT     GPIO_PORT_P1
#define APP_LED1_PIN      GPIO_PIN0

// green
#define APP_LED2_PORT     GPIO_PORT_P1
#define APP_LED2_PIN      GPIO_PIN1

#endif








/*
 * A voltage which ensures a slow-rising power supply (solar)
 * has enough reserve to continue the boot code.
 * Used at coldstart time, to sleep until Vcc reaches this level.
 *
 * This depends on the board: the trigger voltage of the voltage monitor
 * and the size of the power supply storage.
 *
 * Units centivolts
 */
#define MinVccForStarting 190



// GPIO config for pin used for ADC reads
// GPIO_PORT_ADC7, GPIO_PIN_ADC7, GPIO_FUNCTION_ADC7 are used in examples, but undefined
// The module function is moot??? GPIO_PRIMARY_MODULE_FUNCTION
#define ExternalPinVoltagePort        GPIO_PORT_P1
#define ExternalPinVoltagePin         GPIO_PIN7
#define ExternalPinVoltagePinFunction GPIO_SECONDARY_MODULE_FUNCTION


// Constant for configuring ADC
#define    ExternalPinVoltagePinADCSelection   ADC_INPUT_A7

/*
 * Constants defined by chip manufacturer
 */







/*
 * Stepper
 */
#define USE_STEPPER


#define MotorCoilA1Port GPIO_PORT_P1
#define MotorCoilA2Port GPIO_PORT_P1
#define MotorCoilB1Port GPIO_PORT_P1
#define MotorCoilB2Port GPIO_PORT_P1

// consecutive pins of Launchpad EXP-MSP430FR2433
#define MotorCoilA1Pin GPIO_PIN0
#define MotorCoilA2Pin GPIO_PIN5
#define MotorCoilB1Pin GPIO_PIN4
#define MotorCoilB2Pin GPIO_PIN6


/*
 * Which GPIO is used to power a stepper driver is defined for LoadSwitch
 */


// Low side switch

#define LOW_SIDE_SWITCH_PORT  GPIO_PORT_P1
#define LOW_SIDE_SWITCH_PIN   GPIO_PIN7


// High side switch

#define HIGH_SIDE_SWITCH_PORT  GPIO_PORT_P2
#define HIGH_SIDE_SWITCH_PIN   GPIO_PIN1




// PWM out

// msp430fr2433
// P1.1
#define PWM_PORT  GPIO_PORT_P1
#define PWM_PIN   GPIO_PIN1


// Test that required definitions exist
