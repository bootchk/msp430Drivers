
#include "voltBandgapRef.h"

// DriverLib
#include <pmm.h>




void VBG::waitForReady() {
    // spin
    while (not PMM_getVariableReferenceVoltageStatus == PMM_REFGEN_READY) ;
}

void VBG::enable() {
    PMM_enableInternalReference();
}

void VBG::disable() {
    PMM_disableInternalReference();
}
