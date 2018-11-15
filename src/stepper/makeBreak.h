/*
 * Make and Break
 *
 * Included in stepper.cpp
 */


void breakOpen(unsigned int breakBit)
{
    // bit clear
    P1OUT &= ~breakBit;
}

void makeClosed(unsigned int makeBit)
{
    // bit set
    P1OUT |= makeBit;
}



void breakThenMake(unsigned int makeBit, unsigned int breakBit)
{
    breakOpen(breakBit);
    /*
     * Assert both ends of coil shorted to ground, field dumps: coasting
     */
    makeClosed(makeBit);
}

void makeThenBreak(unsigned int makeBit, unsigned int breakBit)
{
    makeClosed(makeBit);
    /*
     * Assert both ends of coil high impedance, field persists: braking
     */
    breakOpen(breakBit);

}

/*
 * bits off in any order
 */
void breakBits(unsigned int bit1, unsigned int bit2) {
    // bit clear
    P1OUT &= ~(bit1 & bit2);
}

void makeBits(unsigned int bit1, unsigned int bit2) {
    // bit set
    P1OUT |=  (bit1 & bit2);
}
