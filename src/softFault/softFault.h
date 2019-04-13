
/*
 * Faults detected in software.
 *
 * Can be from hw failure,
 * or a programming error.
 *
 * Similar to exceptions, but we don't use C++ exception.
 *
 * It might be possible to recover
 * by resetting the external RTC.
 * For now, handle by rebooting entire system.
 */

class SoftFault {
public:
    // external RTC
    static void failSetAlarm();
    static void failClearAlarm();
    static void failReadTime();
    // light sensor, failed assume booting in light
    static void failDetectLight();
    //
    static void failHandleResetReason(unsigned int resetCode);

    static void info(unsigned int infoCode);
};
