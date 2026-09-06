
#include "motorSpeedFeedback.h"
// Pins
#include "board.h"


// DriverLib
#include <timer_b.h>
#include <gpio.h>


// TODO make this a counter instead of a flag
bool countReachedFlag = false;


/*
See board.h for configuration of Timer peripherals

!!! Optional: some boards may not support.
In that case, many functions are no-ops
and no interrupt comes
and wasCountReachedFlag always returns false.

Some packages don't have enough TimerB instances.
Some boards may not route the FG signal from motor driver
to MCU pin.
*/


#if BOARD_HAS_MOTOR_SPEED_FEEDBACK

/*
Init a GPIO to catch the first turn
using interrupt from a low pulse from FG pin of motor driver
*/ 
void 
MotorSpeedFeedback::initPinsForSingleTurn()
{
    countReachedFlag = false;
    
    GPIO_setAsInputPinWithPullUpResistor(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
    GPIO_selectInterruptEdge(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN,
        GPIO_HIGH_TO_LOW_TRANSITION);
}


void 
MotorSpeedFeedback::disableSingleTurnInterrupt()
{
    GPIO_disableInterrupt(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
}

void 
MotorSpeedFeedback::enableSingleTurnInterrupt()
{
    // clear global flag set by ISR
    countReachedFlag = false;

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
    // Set global flag
    countReachedFlag = true;
    // Clear interrupt
    GPIO_clearInterrupt(
       MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN);
    // The interrupt may come again, until we unconfigure the pin
    // or stop motor and motor stops turning.
}

#else

/*
Board not support motor speed feedback

The program must expect wasCountReachedFlag always false,
and do something else (time expires usually.)

The program should at least call initPinsForSingleTurn
because it initializes countReachedFlag which is not persistent in FRAM.
*/
void 
MotorSpeedFeedback::initPinsForSingleTurn()
{
    countReachedFlag = false;
}

void 
MotorSpeedFeedback::disableSingleTurnInterrupt()
{  __no_operation(); }

void 
MotorSpeedFeedback::enableSingleTurnInterrupt()
{  __no_operation(); }

void
MotorSpeedFeedback::handlePinInterrupt()
{ __never_executed(); }

#endif




bool 
MotorSpeedFeedback::wasCountReachedFlag()
{
    return countReachedFlag;
}

