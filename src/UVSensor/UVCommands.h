#pragma once

/*
 * Knows how to read UV sensor registers.
 *
 * The name of each method includes name of the register.
 */
class UVCommands {

private:
    enum UVSensorAddress {
        Identifier =      0x00,
        Input0 =          0x0A,
        Command =         0x0B,
        IRQEnable =       0x0F,
        Response1 =       0x10,
        Response0 =       0x11,
        IRQStatus =       0x12,
        HostOut =         0x13,
        HostOut1 =        0x14,
        HostOut2 =        0x15,
    };


public:
    enum UVCommand {
            ResetCommandCounter = 0x00,
            Reset =               0x01,
            ForceRead =           0x11,
        };


    // Send command, waiting for it to finish
    static unsigned int sendCommand(UVCommand);

    // Convenience
    static unsigned int sendCommandResetCommandCounter();
    static unsigned int sendCommandForceRead();
    static unsigned int sendCommandReset();


    /*
     * get or set a register
     */
    static unsigned int getID(unsigned char *ID);
    static unsigned int setCommand(UVCommand command);
    static unsigned int setInput(unsigned char buffer);
    static unsigned int getResponse0(unsigned char *buffer);
    static unsigned int getResponse1(unsigned char *buffer);
    static unsigned int setParam(unsigned int address);
    static unsigned int getUVSample(long* sample);
    static unsigned int getIRQStatus(unsigned char *status);
    static unsigned int setIRQEnable(unsigned char value);

    // Get response and check the error bit is not set
    static unsigned int getResponseWithErrorCheck(unsigned char *buffer);


    // convienience
    static unsigned int enableIRQChannelZero();



    // Misc

    /*
     * Spin, limited tries, until measurement complete.
     */
    static unsigned int tryChannelZeroMeasureComplete();

    static void sleepWhileReadingChannel();
    /*
     * Do not access the Si1133 earlier than 25 ms from power-up
     */
    static void sleepWhileBooting();
    /*
     * Delay for 10 ms. This delay lets the Si1133 perform internal reset sequence.
     */
    static void sleepWhileResetting();

    /*
     * Wait limited tries for Response to show device is sleeping.
     *
     * Better to use IRQ.
     * A more sensitive reading takes more time.
     * You must experiment to know how many loops it normally takes to read.
     */
    static unsigned int waitUntilSleep(unsigned int maxLoopCount);



private:
    static unsigned int waitForConsistentCommandCount(unsigned int * );
    static unsigned int waitForChangedCommandCount(unsigned int priorCommandCount, unsigned int maxLoopCount);

};
