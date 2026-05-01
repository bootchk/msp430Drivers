
#pragma once

#include <cstdint>

class PeriodicInterrupt {
private:
    static void initInTicks(uint16_t ticks);
public:
    static void initInSeconds(uint16_t seconds);
};
