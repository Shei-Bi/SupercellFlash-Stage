#pragma once
#include "data/LogicGearBoostData.h"
#include "network/BitStream.h"

class LogicGear {
public:
    LogicGearBoostData* data;
    int type;
    bool isActive;
    int shieldAmount;
    LogicGear(LogicGearBoostData* data) :data(data) {
        type = data->getLogicType();
    }
    void decode(BitStream* stream, bool isOwn) {
        isActive = stream->readBoolean();
        if (type == 4)
            shieldAmount = stream->readPositiveIntMax1023();
    }
};