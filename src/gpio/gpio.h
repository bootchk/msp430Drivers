
/*
 * A GPIO pin
 *
 * Many instances.
 *
 * A thin wrapper around
 */

class Gpio {
public:
    unsigned int port;
    unsigned int pin;

    Gpio(unsigned int, unsigned int);

    void configAsOutput();



};
