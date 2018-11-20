
#include "powerMgtModule.h"

// depends on register symbols
#include <msp430.h>

// mcu hal layer e.g. MSPWare DriverLib
// depends on msp430.h
#include <pmm.h>

// some mcu family members have watchdog a and b
// #include <wdt_a.h>

#include "../assert/myAssert.h"




void PMM::configureOff() {

	// This saves a little more power.  The mode is not named.
	PMM_disableSVSH();

	// This gets x.5 power mode
	PMM_turnOffRegulator();
}




void PMM::unlockLPM5() {
	PMM_unlockLPM5();

	// ensure
	myAssert(not isLockedLPM5());
}

bool PMM::isLockedLPM5() {
    // is lockplm5 bit set?
    return (PM5CTL0 & LOCKLPM5);
}





bool PMM::isResetAWakeFromSleep() {
	return PMM_getInterruptStatus(PMM_LPM5_INTERRUPT);
}


void PMM::clearIsResetAWakeFromSleep() {
	PMM_clearInterrupt(PMM_LPM5_INTERRUPT);
}


void PMM::clearAllResetInterruptFlags() {
    PMM_clearInterrupt(PMM_ALL);
}

void PMM::triggerSoftwareBORReset() {
    // !!! BOR is distinct from POR.  BOR is equivalent to power up reset
    PMM_trigBOR();
}
