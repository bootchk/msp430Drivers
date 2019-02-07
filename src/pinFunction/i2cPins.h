
#pragma once


class I2CPins {

public:
    /*
     * Configure all pins needed for I2C (only two, sometimes called "twi" for "two-wire interface"
     *
     * No pins (signals) dedicated to slave select.
     */

    /*
     * Ensures pins dedicated to I2C module and pullups appropriate to board design.
     */
    static void configure();

    /*
     * Ensures pins appropriate for idle state of bus (SCLK and SDA high)
     */
    static void unconfigure();
};
