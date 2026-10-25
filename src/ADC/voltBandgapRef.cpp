
#include "voltBandgapRef.h"

// DriverLib
#include <pmm.h>



/*
Busy wait until reference voltage generator is stable.

Implementation is via reference generator.
Not sure whether we also need to check readiness of VBG.
*/
void VBG::waitForInternalVoltageReferenceReady() {
    // spin
    while (not PMM_getVariableReferenceVoltageStatus == PMM_REFGEN_READY) ;
}


/*
Internal voltage reference, that can be read by ADC.
AKA "variable" because a voltage can be selected, 1.5V, 2.5V, ...
Defaults to 1.5V
*/
void VBG::enableInternalVoltageReference() {
    PMM_enableInternalReference();
}

void VBG::disableInternalVoltageReference() {
    PMM_disableInternalReference();
}


/*
Enable the 1.2V reference (aka bandgap)
to reach an external pin.
On FR2311, this pin can also be read by the ADC as channel 7.
It is buffered, i.e. adapted to external pin.
It is not "variable" but always 1.2V, unlike the other voltage ref,
which can be selected as 1.5V, 2.5V, etc. on some models.
*/
void 
VBG::enable1_2VReference()
{
    PMM_enableExternalReference();
}
