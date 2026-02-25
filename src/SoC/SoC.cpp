
#include <msp430.h>
#include <board.h>


// TI driverlib
#include <sysctl.h>
#include <cs.h>

#include "../SoC/SoC.h"
#include "../PMM/powerMgtModule.h"
#include "../resetReason/resetReason.h"


#ifdef DISABLE_BSL
/*
 * Disable BSL
 * BSL might already have executed, this only affects subsequent resets.
 */
#pragma location=0xFF84
unsigned int bslSignature1=0x5555;
#pragma location=0xFF86
unsigned int bslSignature2=0x5555;
#endif




void SoC::stopWatchDog() {
    // Using Driverlib
    //WDT_A_hold();
    WDTCTL = WDTPW | WDTHOLD;
}


/*
Enters LPM4 or LPM4.5 with general interrupts enabled.

DOES NOT RETURN

Whether is 4 or 4.5 depends on prior step
of call to PMM::configureOff to turn power off.
Such a call must be immediately before,
since only capacitors hold up Vcc long enough
for the CPU to execute this call.

Discussion:
	- LPM4 macro
    __bis_SR_register(LPM4_bits)
These do not atomic set GIE
*/
void SoC::enterLPM4orLPM4_5(){
	/*
	Set certain bits in the mcu status register (SR)
	Atomically enable global interrupts and sleep.
	*/
	__bis_SR_register(LPM4_bits & GIE);
    __no_operation();
}

void SoC::enterLPM3orLPM3_5(){
	/*
	Set certain bits in the mcu status register (SR)
	Atomically enable global interrupts and sleep.
	*/
	__bis_SR_register(LPM3_bits & GIE);
    __no_operation();
}


void SoC::enableGlobalInterrupts() {
    __bis_SR_register(GIE);
}



void SoC::enableBSLOffAndVacantMemoryNMI() {

    // Clear NMI and VMA flags
    // so we don't get an immediate interrupt if one has already occurred.
    SFRIFG1 &= ~(NMIIFG | VMAIFG);

    // vacant memory generate interrupt as well as read and execute funny
    // bit set
    SFRIE1 |= VMAIE;

#ifdef __MSP430FR2433__
    // BSL memory behave as vacant memory
    // bit set
    SYSBSLC |= SYSBSLOFF;
#endif

}


void SoC::disableFRAMWriteProtect() {
#ifdef __MSP430FR2433__
    // By default, writes cause NMI.  To disable, enable writing.
    SysCtl_enableFRAMWrite(SYSCTL_FRAMWRITEPROTECTION_DATA | SYSCTL_FRAMWRITEPROTECTION_PROGRAM);
#endif
}


/*
 * Delegate to PMM
 */
void SoC::clearIFGForResetWakeFromSleep() { PMM::clearIsResetAWakeFromSleep(); }
void SoC::unlockGPIOFromSleep(){ PMM::unlockLPM5(); }
void SoC::triggerSoftwareReset() { PMM::triggerSoftwareBORReset(); }

/*
 * Delegate to ResetReason
 * (Another version delegates to PMM)
 */
bool SoC::isResetWakeFromSleep() { return ResetReason::isResetAWakeFromSleep(); }



void SoC::disableXT1() {
    /*
    Macro resolves to either:
    CS_turnOffXT1(); FR2xx
    CS_turnOffLFXT(); FRxx
    depending on target
    */

    disableLowXT();
}

void SoC::turnOffSMCLK()  { CS_turnOffSMCLK(); }


