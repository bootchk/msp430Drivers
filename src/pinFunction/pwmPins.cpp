#include "pwmPins.h"

// board.h comes from App or from test harness directory
#include <board.h>


void PWMPins::configure()
{
    // msp4302433 
    // Route pin P1.1 to output of second module
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        PWM_PORT,   PWM_PIN,  
        GPIO_SECONDARY_MODULE_FUNCTION);

    // Assert direction is out
    // Since the above is "OutputPin"
};

void
PWMPins::configureToDigitalLow()
{ 
    GPIO_setOutputLowOnPin(PWM_PORT, PWM_PIN);
    GPIO_setAsOutputPin(PWM_PORT, PWM_PIN);
}

