/*
 * A set of segments of eInk display
 *
 * Knows:
 * - top segment versus remaining subset of visible segments
 * - how to iterate over visible segments
 *
 * Currently a singleton: static members.
 *
 * Owns an array of Segment.
 *
 * All operations return immediately.
 */
class SegmentSet {
public:

    /*
     * Requires run-time init, once at initial POR power on reset.
     */
    static void configure();

    // Configure GPIO pins for segments
    static void configurePinsOut();
    static void configurePinsInGrounded();

    // Visible segments are subset of all segments
    static void allVisibleSegsLow();
    static void allVisibleSegsHigh();

    // Top segment is unique (distinguished) element of set of all segments
    static void driveTopLow();
    static void driveTopHigh();

    // Visible segments
    // Seg 0 is the background, seg 1... are the bars of the bargraph
    static bool isSegmentOn(unsigned int segment);
    static void setSegmentOn(unsigned int segment, bool value);

    static void driveSegmentHigh(unsigned int segment);
    static void driveSegmentLow(unsigned int segment);
};
