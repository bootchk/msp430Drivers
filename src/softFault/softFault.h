
/*
 * Faults detected in software.
 *
 * Can be from hw failure,
 * or a programming error.
 *
 * For now, all failures on the external RTC.
 *
 * It might be possible to recover
 * by resetting the external RTC.
 * For now, handle by rebooting entire system.
 */

class SoftFault {
public:
    static void failSetAlarm();
    static void failClearAlarm();
    static void failReadTime();
};
