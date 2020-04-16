#include "UVSensor.h"

#include "UVSensorRegisters.h"

#include "../bridge/bridge.h"




bool UVSensor::isSane() {
    unsigned char ID;

    bool success = Bridge::readByte(static_cast<unsigned char>(UVSensorAddress::Identifier), &ID);
    return (ID == VEML6075Identifier);
}
