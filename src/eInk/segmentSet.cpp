
#include "segmentSet.h"

#include "segment.h"

// DriverLib used in implementation (for pin name constants)
// Platform dependent.
#include <gpio.h>



#define COUNT_SEGMENTS 3


#pragma RETAIN
#pragma PERSISTENT
static int foobar = 1;



/*
 * These are persistent (in FRAM) but not initialized at load time.
 * You must call configure() once during lifetime, to configure at runtime, at POR.
 *
 * The #pragma PERSISTENT requires compile time initialization, unfortunately, so can't be used easily here.
 */
#pragma DATA_SECTION( ".TI.persistent" )
Segment segTop;

#pragma DATA_SECTION( ".TI.persistent" )
static Segment segments[COUNT_SEGMENTS];






void SegmentSet::configure() {
    segTop = Segment(GPIO_PORT_P1, GPIO_PIN0, false);

    segments[0] = Segment(GPIO_PORT_P1, GPIO_PIN2, false);
    segments[1] = Segment(GPIO_PORT_P1, GPIO_PIN3, false);
    segments[2] = Segment(GPIO_PORT_P1, GPIO_PIN4, false);
    // TODO more segments
}



void SegmentSet::configurePinsOut() {
    segTop.setOutput();

    for (int i=0; i<COUNT_SEGMENTS; i++) {
        segments[i].setOutput();
    }
}

void SegmentSet::configurePinsInGrounded() {
    segTop.setInput();

    for (int i=0; i<COUNT_SEGMENTS; i++) {
        segments[i].setInput();
    }
}




void SegmentSet::allVisibleSegsLow() {
    for (int i=0; i<COUNT_SEGMENTS; i++) {
        segments[i].driveLow();
    }
}

void SegmentSet::allVisibleSegsHigh() {
    for (int i=0; i<COUNT_SEGMENTS; i++) {
        segments[i].driveHigh();
    }
}

void SegmentSet::driveTopLow() { segTop.driveLow(); }
void SegmentSet::driveTopHigh() { segTop.driveHigh(); }

bool SegmentSet::isSegmentOn(unsigned int segment){ return segments[segment].isOn(); }
void SegmentSet::setSegmentOn(unsigned int segment, bool value){ segments[segment].setOn(value); }

void SegmentSet::driveSegmentHigh(unsigned int segment){ segments[segment].driveHigh(); }
void SegmentSet::driveSegmentLow(unsigned int segment){ segments[segment].driveLow(); }


