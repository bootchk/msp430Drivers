
#include "segment.h"


// DriverLib used in implementation
// Platform dependent.
#include <gpio.h>



Segment::Segment(unsigned int port, unsigned int pin, bool isOn){
    _port = port;
    _pin = pin;
    _isOn = isOn;
}


bool Segment::isOn(){ return _isOn; }

void Segment::setOn(bool value) { _isOn = value; }



void Segment::setOutput() { GPIO_setAsOutputPin(_port, _pin); }
void Segment::setInput(){ GPIO_setAsInputPinWithPullDownResistor(_port, _pin); }
// Alternatively: floating input: GPIO_setAsInputPin(


void Segment::driveHigh(){ GPIO_setOutputHighOnPin(_port, _pin); }

void Segment::driveLow(){ GPIO_setOutputLowOnPin(_port, _pin); }


