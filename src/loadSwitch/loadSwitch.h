
/*
 * High side load switch.
 */

class HighSideSwitch {
public:
    static void configure();    /* configure GPIO pin according to board.h */
    static void turnOn();
    static void turnOff();
};
