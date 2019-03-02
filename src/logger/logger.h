/*
 * Logs to persistent storage.
 * Log is useful for post-mortem of failed production system (where typically system software resets itself.)
 * Typically logs unrecoverable softfaults, brownout condition, and failed assertions.
 *
 * Persistent storage is FRAM.
 *
 * Log survives a reset.
 * Log does not survive a program load using debugger.
 * Log survives a debug connection (to running system) when program is not loaded, only symbols.
 *
 * Logs until the log is full, then quietly returns.
 *
 * Log is cleared by a program load using debugger (which loads log FRAM with zeros.)
 *
 * Requires FRAM not write protected, else will fail silently, or generate NMI
 */

class Logger {
public:
    /*
     * Typically debugging, might be just a trace.
     */
    static void log(unsigned int);

    /*
     * Severe error.
     * Does not return.
     */
    static void logError(unsigned int);
};
