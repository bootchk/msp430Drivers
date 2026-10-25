
/*
 * API to control the VBG module
 *
 * Note the MSP430FR2433 has a "basic" voltage reference, not "enhanced".
 *
 * Thus 2.0, 2.5V references not available.
 * Also, no BGMODE appears to be available.
 */

class VBG {
public:
    static void enableInternalVoltageReference();
    static void disableInternalVoltageReference();
    
    static void waitForInternalVoltageReferenceReady();

    static void enable1_2VReference();
};
