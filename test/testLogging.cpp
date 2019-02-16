
#include "../src/SoC/SoC.h"

#include "../src/assert/myAssert.h"



void testLogging() {

    // must be disabled for logging to write to FRAM
    SoC::disableFRAMWriteProtect();

    myAssert(false);
    // expect 3 to be written to log e.g. at address 0xc400
}
