


/*
 * State machine for eUSCI_B using I2C.
 *
 * The eUSCI_B is a peripheral device.
 * It is in certain states.
 * This class models those states.
 * It accepts events (interrupts) from the eUSCI_B device to transition between states.
 * This state machine runs in the interrupt state (implemented mostly as an ISR.)
 *
 * Class is static, a singleton.
 *
 * This is not the link layer, but a layer between.
 * The peripheral implements the link layer i.e. turning the bus around for read and writes across the link.
 * The state machine knows when to turn the link around.
 */


class I2CStateMachine {
public:
    // TODO document the parameters
    static void init(
            uint8_t registerAddress,
             uint8_t * const buffer,
             const uint8_t count,
             const bool isSend);

    /*
     * The initial transition is to send [START, slave address]
     *
     * Subsequent transitions are as specified by init() (whether read/write, etc.)
     *
     * This does not return until the stateMachine enters Idle state again.
     * At that time, buffer might have contents of the I2C transaction (for a read.)
     * Meanwhile, mcu sleeps in LPM while waiting for transitions from eUSCI_B device.
     */
    static void initialTransition();

    /*
     * Spin (busy wait) until any STOP has been transmitted for prior transaction.
     * Avoid a race.
     */
    static void waitUntilPriorTransportComplete();
};

