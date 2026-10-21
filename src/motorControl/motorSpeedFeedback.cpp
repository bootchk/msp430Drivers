
#include "motorSpeedFeedback.h"
// Pins
#include "board.h"


// DriverLib
#include <timer_b.h>
#include <gpio.h>


bool         didTurnFlag = false;
unsigned int turnCount = 0;


/*
See board.h for configuration of Timer peripherals

!!! Optional: some boards may not support.
In that case, many functions are no-ops
and no interrupt comes
and didTurn always returns false.

Some packages don't have enough TimerB instances.
Some boards may not route the FG signal from motor driver to MCU pin.
*/


#if BOARD_HAS_MOTOR_SPEED_FEEDBACK

/*
Init a GPIO to catch first and subsequent turns
using interrupt from a low pulse from FG pin of motor driver
*/ 
void 
MotorSpeedFeedback::initPinsForTurnInterrupt()
{
    didTurnFlag = false;  // TODO don't need this
    
    GPIO_setAsInputPinWithPullUpResistor(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
    GPIO_selectInterruptEdge(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN,
        GPIO_HIGH_TO_LOW_TRANSITION);
}


void 
MotorSpeedFeedback::disableTurnInterrupt()
{
    GPIO_disableInterrupt(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
}

void 
MotorSpeedFeedback::enableTurnInterrupt()
{
    // clear vars set by ISR
    didTurnFlag = false;
    turnCount   = 0;

    // Clear before enabling
    GPIO_clearInterrupt(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
    GPIO_enableInterrupt(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
}


void 
MotorSpeedFeedback::handlePinInterrupt()
{
    // Set global vars
    didTurnFlag = true;
    turnCount++;

    // Clear interrupt
    GPIO_clearInterrupt(
       MOTOR_CONTROL_PORT, 
       MOTOR_CONTROL_PIN);
    // The interrupt may come again, until we disable interrupt
    // or stop motor and motor stops turning.
}

#else

/*
Board not support motor speed feedback

The program must expect didTurn always false,
and do something else (time expires usually.)

The program should at least call initPinsForSingleTurn
because it initializes didTurnFlag which is not persistent in FRAM.
*/
void 
MotorSpeedFeedback::initPinsForTurnInterrupt()
{
    didTurnFlag = false;
}

void 
MotorSpeedFeedback::disableTurnInterrupt()
{  __no_operation(); }

void 
MotorSpeedFeedback::enableTurnInterrupt()
{  __no_operation(); }

void
MotorSpeedFeedback::handlePinInterrupt()
{ __never_executed(); }

#endif


/*
!!!
If board has no motor speed feedback:
    didTurn() returns false
    getTurnCount returns zero
(And usually a program would turn motor on for a certain period.)
*/

bool 
MotorSpeedFeedback::didTurn()
{
    return didTurnFlag;
}


/*
!!! This is not revolutions, but electrical commutation ticks,
which depends on pole count.
E.G. 16 commutations for an 8 pole motor is two revolutions.
Note the driver IC can also divide by two.
*/
unsigned int 
MotorSpeedFeedback::getTurnCount()
{
    return turnCount;
}

