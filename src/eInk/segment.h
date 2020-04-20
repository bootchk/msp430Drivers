

/*
 * A segment of eInk display
 *
 * Knows:
 * - Persistent visibility state (on/off)
 * - GPIO port and pin
 * - drive high and low
 *
 * On/off is not necessarily white/black.
 * Drive high/low is not necessarily on/off
 * We can drive high/low even if not changing on/off (depends on the setting of top electrode.)
 *
 * !!! Hack: used for visible and invisible segments.
 * The top plate (sic "segment") is invisible (transparent), and overlaps (covers) all visible segments.
 * All visible segments do not overlap.
 * Using Segment for the top plate means it has a state variable, but it really doesn't have a visible state.
 *
 *
 * Setting state is distinct from driving.
 * You may need to drive segment low or high to get in on, and low or high to get it off.
 * Unlike an LED, drive high is NOT always On.
 *
 * All operations return immediately.
 * It is only higher level ops that must delay for ink to change color.
 */
class Segment {

/*
 * A Segment instance should be persistent, if you use LPM4.5 that resets memory.
 * These class attributes are persistent if the Segment instances are.
 */
bool _isOn;
unsigned int _port;
unsigned int _pin;

public:
    // !!! default constructor, with default values for all parameters
    Segment(unsigned int port=0, unsigned int pin=0, bool isOn=false);

    /*
     * Configure segment's GPIO pin.
     *
     * To save power, you can set to an input.
     * Alternatively, you can leave as an output, and drive all pins to the same voltage (Vcc or ground.)
     * The choice can depend on whether you use LPM4.5
     */
    void setOutput();
    void setInput();

    // Change visibility state
    bool isOn();
    void setOn(bool);

    // Drive to Vcc or ground voltage
    void driveHigh();
    void driveLow();
};
