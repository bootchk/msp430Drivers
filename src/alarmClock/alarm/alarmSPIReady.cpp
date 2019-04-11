
/*
 * Unused cruft
 *
 * From a design where we wait for the FOUT/nIRQ pin high to signal RTC ready
 */


#ifdef NOT_USED

/*
 * RTC signals SPI not ready (during reset) by asserting rtc:Fout/nIRQ pin low.
 * !!! Same pin as for the Alarm signal.
 *
 * Since SPI ready uses the alarm pin, it depends on interrupt is a pulse on rising edge.
 * Alternatively, if signal is another RTC pin (nRST), the interrupt could be configured falling edge.
 */
bool Alarm::isSPIReady() {
    require(isConfiguredMcuAlarmInterface());
    return (Alarm::isAlarmInterruptSignalHigh());
}


void Alarm::resetIfSPINotReady() {
    if (!Alarm::isSPIReady()) {
         // System is in invalid state (mcu not POR, but rtc is POR)
         PMM::triggerSoftwareBORReset();
     }
}




void Alarm::waitSPIReadyOrReset() {
    /*
     * Require RTC in reset condition:  OUT bit is 1 and FOUT/nIRQ configured to show OUT bit.
     * Otherwise, this code will reset.
     */
    /*
     * Does NOT require SPI interface is ready, only requires alarm pin configured on MCU side.
     */
    // WRONG assert(RTC::readOUTBit() == true);

    int i = 0;
    while ( ! Alarm::isSPIReady() ) {
        i++;
        if (i > 1000) {
            PMM::triggerSoftwareBORReset();
        }
    }
}
#endif

#ifdef OLD
            /*
             * Spin finite time waiting for rtc ready for SPI, i.e. out of reset.
             */
            // TODO TEMP
            //Alarm::waitSPIReadyOrReset();

            // assert alarm interrupt signal is high
            // mcu pin resets to an input, but without interrupt enabled

            // Assume rtc was reset also
#endif
