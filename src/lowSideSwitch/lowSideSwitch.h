/*
 * GPIO out driving an NMOS FET to switch a ground to a load.
 *
 * Drain of FET is connected to a load ground (low side.)
 * Load high side is connected power rail say 5V.
 * Thus drain of FET is always high.
 *
 * Gate of FET when logic low is switch off (FET is open and no current through it.)
 *
 * Source of FET is connected to ground.
 *
 * Gate of FET when logic high (say 3.3V) is switch on (FET is closed and current flows through it.)
 * Current flows from power rail through load through FET to ground.
 *
 * Note the gate should have a pull down resistor of say 10k ohms to insure capacitance on gate bleeds away
 * if not driven low (say left floating.)
 *
 * Gate might have a 100 ohm current limiting resistor.
 */


class LowSideSwitch {
public:
    static void configure();    /* configure GPIO pin according to board.h */
    static void turnOn();
    static void turnOff();
};
