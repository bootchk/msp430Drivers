#include "pwmPins.h"

// board.h comes from App or from test harness directory
#include <board.h>


void PWMPins::configure()
{
    // Port, pin, and function defined in board.h
    // Varies with MSP430 model
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        PWM_PORT,   
        PWM_PIN,  
        PWM_MODULE_FUNCTION);

    // Direction is out since called func is "OutputPin"
};

/*
Equivalent to OFF for most PWM uses.
No pulses.
*/
void
PWMPins::configureToDigitalLow()
{ 
    GPIO_setOutputLowOnPin(PWM_PORT, PWM_PIN);
    GPIO_setAsOutputPin(PWM_PORT, PWM_PIN);
}

/*
Equivalent to 100% duty cycle.
No pulses, unless you consider high state a pulse.
For speed control, means full speead.
*/
void
PWMPins::configureToDigitalHigh()
{ 
    GPIO_setOutputHighOnPin(PWM_PORT, PWM_PIN);
    GPIO_setAsOutputPin(PWM_PORT, PWM_PIN);
}


