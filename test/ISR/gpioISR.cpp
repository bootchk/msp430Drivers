/*
 * ISR for a PORT1 GPIO pin
 */

#include "../../src/assert/myAssert.h"



#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    myAssert(false);
}
