Test harnesses and supporting code for msp430Drivers

Note that ISR's in the supporting code correspond to test conditions (LPM3 instead of LPM4.5)
In some uses of msp430Drivers (when used by embeddedDutyCycle project) these ISR's are not appropriate.
Since embeddedDutyCycle uses LPM4.5 instead of LPM3.