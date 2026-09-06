#include "motorControl.h"
// Pins
#include "board.h"


// DriverLib
#include <timer_b.h>
#include <gpio.h>


/* This implementation counts turns.
Uses FG signal from motor as clock to timer/counter peripheral.

!!! It might not work.
I tested, inconclusively: maybe needed a pullup on FG net
to clean up the signal, or maybe this will never work.
*/


#if MOTOR_CONTROL_IS_COUNTING

// Configure pin as input from motor driver to TimerB clock
// The signal name is abstract, referred to elsewhere.
// The driver IC DRV10866.FG is open drain, needs pull up.
void 
MotorControl::initPinsForTurnCount()
{
    // Must configure pullup and module function separately.

    // Configure pull up
    GPIO_setAsInputPinWithPullUpResistor(MOTOR_CONTROL_PORT, MOTOR_CONTROL_PIN);
    // Choose the use as a clock for timer
    // This is the first, or primary, module function.
    // The primary is NOT as I/O
    GPIO_setAsPeripheralModuleFunctionInputPin(
        MOTOR_CONTROL_PORT, 
        MOTOR_CONTROL_PIN,
        MOTOR_CONTROL_MODULE_FUNCTION);
}


void 
MotorControl::startTurnCounter( int turnsToReach, int polePairs)
{
    /*
    Require GPIO pin configured to function as input to internal signal TXCLK,
    which will clock the timer.

    Up mode: timer counts its clock until matches CCR0
    */

    countReachedFlag = false;
    
    Timer_B_initUpModeParam config = {0};
    
    // Uses first CCR0, don't need an output

    // Clock source is a GPIO pin secondary function IN
    config.clockSource                 = TIMER_B_CLOCKSOURCE_EXTERNAL_TXCLK;
    config.clockSourceDivider          = TIMER_B_CLOCKSOURCE_DIVIDER_1;

    // disable  interrupt from overflow
    config.timerInterruptEnable_TBIE   = TIMER_B_TBIE_INTERRUPT_ENABLE;
    // enable interrupt due to CCR0
    config.captureCompareInterruptEnable_CCR0_CCIE = TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    // Clear now
    config.timerClear                  = TIMER_B_DO_CLEAR;
    // Start now
    config.startTimer                  = true;
   
    // trigger value
    // Multiply by polePairCount: 4 for Maxon EC9.2
    config.timerPeriod                 = turnsToReach * polePairs;
    Timer_B_initUpMode(
        MOTOR_CONTROL_TIMER_BASE,
        &config);

    // !! Some docs say the above does NOT start timer
    Timer_B_startCounter(MOTOR_CONTROL_TIMER_BASE, TIMER_B_UP_MODE);

    // Require motor not started and not generating pulses to timer clock
    // When motor starts, counter will interrupt after turnsToReach and set flag.
    // Must poll the flag, and can turn off motor.
}


void
MotorControl::stopTurnCounter()
{
    Timer_B_stop(MOTOR_CONTROL_TIMER_BASE);
    Timer_B_disableInterrupt(MOTOR_CONTROL_TIMER_BASE);
}


void 
MotorControl::handleTimerInterrupt()
{
    // Set global flag
    countReachedFlag = true;
    // And stop counter.  Alternatively, let caller stop it.
    MotorControl::stopTurnCounter();
}

#endif