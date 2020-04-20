
#include "eInkBargraph.h"

#include "segmentSet.h"

/*
 * Implementation is independent of HAL.
 * That is, it doesn't include the library that implements GPIO for specific hardware.
 *
 * It is abstractly about the eInk device.
 * It knows the steps are:
 *  configure pins
 *  drive top segment and all visible segments
 *  delay for ink settling
 *  undrive and unconfigure pins for final state
 *
 * Knows only that the eInk device has a SegmentSet.
 * Does not know Segment directly.
 */


#define EINK_USE_TIMER 1




#ifndef EINK_USE_TIMER
static void delayHalfSecond() {
    __delay_cycles(500000); // half second
}
#else
#include "../timer/timer.h"
#endif





void EInkBargraph::delayForInkSettling() {
    // TODO clear flag

#ifdef EINK_USE_TIMER
    LowPowerTimer::delaySeconds(1);
#else
    // Two seconds.
    // eInk specifies 0.5 to 2 seconds at 5V
    delayHalfSecond();
    //delayHalfSecond();
    //delayHalfSecond();
    //delayHalfSecond();
#endif

    EInkBargraph::endInking();
}


void EInkBargraph::endInking() {
    SegmentSet::configurePinsInGrounded();
    // TODO set flag
    // ensure no pins are driven, GPIO pins are configured inputs
}




void EInkBargraph::configure() {
    // Do this first, so segments know their pins, and state is off
    SegmentSet::configure();

    // Drive all white to correspond with configured state off
    allWhite();
    // ensure device is all white, pins are defined, and internal state is off
}






void EInkBargraph::allWhite() {
    SegmentSet::configurePinsOut();

    // One plate high, all opposite plates low
    SegmentSet::driveTopHigh();
    SegmentSet::allVisibleSegsLow();

    delayForInkSettling();
}




void EInkBargraph::allBlack() {
    SegmentSet::configurePinsOut();

    // Top low, all segments high
    SegmentSet::driveTopLow();
    SegmentSet::allVisibleSegsHigh();

    delayForInkSettling();
}



void EInkBargraph::toggleSegment(unsigned int segment) {

   SegmentSet::configurePinsOut();

    if (SegmentSet::isSegmentOn(1)) {
        // Top low, desired segment high
        SegmentSet::driveTopLow();

        // First all segs low (same as top), afterwards change desired seg
        SegmentSet::allVisibleSegsLow();

        SegmentSet::driveSegmentHigh(segment);

        SegmentSet::setSegmentOn(segment, false);
    }
    else {
        // Top high, desired segment low
        SegmentSet::driveTopHigh();
        SegmentSet::allVisibleSegsHigh();
        SegmentSet::driveSegmentLow(segment);
        SegmentSet::setSegmentOn(segment, true);
    }
    delayForInkSettling();
}
