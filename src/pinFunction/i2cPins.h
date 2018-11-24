
#pragma once


class I2CPins {

public:
    /*
     * Configure all pins needed for I2C (only two, sometimes called "twi" for "two-wire interface"
     *
     * No pins (signals) dedicated to slave select.
     */
    static void configure();
    static void unconfigure();
};
