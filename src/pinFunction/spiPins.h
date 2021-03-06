
#pragma once


class SPIPins {
private:

    static void configureDataSPIPins();


public:
    /*
     * pin, which is active during sleep (not selected state.)
     */
    static void configureSelectSlavePin();

    /*
     * Configure all pins needed for SPI
     */
    static void configure();
    static void unconfigure();

    /*
     * Function of the pin is to select a slave device.,
     * from possibly many on the SPI bus.
     */
    static void selectSlave();
    static void deselectSlave();
};
