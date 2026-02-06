#include "pwmPins.h"

// board.h comes from App or from test harness directory
#include <board.h>



void PWMPins::configure() {
    // msp4302433 
    // Choose P1.1 second module
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        PWM_PORT,   PWM_PIN,  
        GPIO_SECONDARY_MODULE_FUNCTION);

    // Assert direction is out
    // Since the above is "OutputPin"
};
