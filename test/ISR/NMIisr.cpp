
#include <msp430.h>

// msp430Drivers
//#include <SoC/SoC.h>
//#include <assert/fatal.h>

//#include "../test/test.h"


/*
 * We don't use NMI for useful work.
 * All NMI are serious faults that we want to catch.
 *
 * For debugging, blink an LED
 * For production, do software reset.
 */

// JTAG mailbox
// Not enabled by JMBINIE and JMPOUTIE
//if (SFRIFG1 & JMBOUTIFG) assert(false);
//if (SFRIFG1 & JMBINIFG) assert(false);

// Vacant memory access
//if (SFRIFG1 & VMAIFG) assert(false);

// Fram bit errors
// Not enabled by GCCTLO: CBDIE, UBDIE
// GCCTL1: CBDIFG, UBDIFG


#ifdef SAVE_LASTPC
// require disabled FRAM write protection

// Store the address before the fault occurred
// On interrupt, two words of address are pushed. First word bits 16:13 are address bits 19:16
#pragma PERSISTENT
unsigned int lastPC = 0;
#pragma PERSISTENT
unsigned int lastSR = 0;
#endif


#pragma vector = SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
{
#ifdef SAVE_LASTPC
    /*
     * Try to save the PC when VMA occurred.
     * Is futile if VMA was a fetch of instruction,
     * since then the PC is already corrupted.
     */
    // Enable FRAM write for persistent lastPC
    MCU::disableFRAMWriteProtect();

    /*
     * Use assembly language.
     * Note space before the instruction mnemonic.
     * Save the address to be returned to in a FRAM location.
     * The address was pushed onto the stack, then the SR,
     * so the address is SP + 2
     */
   asm( " mov 46(SP), &lastPC");
   asm( " mov 48(SP), &lastSR");
#endif

  while (true) {
    // NMI source
    unsigned int reason = SYSSNIV;
    // see data sheet under SYS module for values.  0x12 is VMA

    switch(reason) {
    case SYSSNIV_NONE:         /* No Interrupt pending */
    case SYSSNIV_VMAIFG:       /* SYSSNIV : VMAIFG */
        // Common case, in my experience



    // FR2433 case SYSSNIV_SVSLIFG:      /* SYSSNIV : SVS low-power reset entry */

    case SYSSNIV_UBDIFG:       /* SYSSNIV : FRAM Uncorrectable bit Error */
    case SYSSNIV_CBDIFG:      /* SYSSNIV : FRAM Correctable Bit error */
    // FR2433 case SYSSNIV_ACCTEIFG:     /* SYSSNIV : ACCTEIFG */

    case SYSSNIV_JMBINIFG :    /* SYSSNIV : JMBINIFG */
    case SYSSNIV_JMBOUTIFG:    /* SYSSNIV : JMBOUTIFG */
    default:
        // Uncommon, in my experience
        // In production, should do SW reset
        while(true);
    }
  }
}
