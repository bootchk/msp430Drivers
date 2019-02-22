
#include "allPins.h"

#include <msp430.h>

void AllPins::setHighOutput() {
    P1OUT = 0xFF;
    P1DIR = 0xFF;

    P2OUT = 0xFF;
    P2DIR = 0xFF;

    P3OUT = 0xFF;
    P3DIR = 0xFF;
}
