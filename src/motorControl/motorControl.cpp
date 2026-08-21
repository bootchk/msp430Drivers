
#include "motorControl.h"
// Pins
#include "board.h"

// DriverLib
#include <timer_b.h>
#include <gpio.h>

static bool countReachedFlag = false;

// Use B0 for motor control IN, use B1 for PWM to motor OUT
#define MOTOR_CONTROL_TIMER_BASE TIMER_B0_BASE

// Configure pin as input from motor driver to TimerB clock
// The signal name is abstact, referred to elsewhere.
// The driver IC DRV10866.FG is open drain, needs pull up.
void 
MotorControl::initPins()
{
    GPIO_setAsInputPinWithPullUpResistor(MOTOR_CONTROL_PORT, MOTOR_CONTROL_PIN);
    
}


void 
MotorControl::startTurnCounter( int turnsToReach)
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

    // enable immediate interrupt
    config.timerInterruptEnable_TBIE   = TIMER_B_TBIE_INTERRUPT_ENABLE;
    // enable interrupt due to CCR0
    config.captureCompareInterruptEnable_CCR0_CCIE = TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    // Clear now
    config.timerClear                  = TIMER_B_DO_CLEAR;
    // Start now
    config.startTimer                  = true;
   
    // trigger value
    // Multiply by polePairCount: 4 for Maxon EC9.2
    config.timerPeriod                 = turnsToReach * 4;

    Timer_B_initUpMode(
        MOTOR_CONTROL_TIMER_BASE, //TIMER_BASE_ADDRESS, 
        &config);

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
MotorControl::handleInterrupt()
{
    // Set global flag
    countReachedFlag = true;
    // And stop counter.  Alternatively, let caller stop it.
    MotorControl::stopTurnCounter();
}

bool 
MotorControl::wasCountReachedFlag()
{
    return countReachedFlag;
}

