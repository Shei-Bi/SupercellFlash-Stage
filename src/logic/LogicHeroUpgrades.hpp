#pragma once
#include "network/ByteStream.h"
#include "data/LogicCardData.h"
#include "data/LogicAccessoryData.h"
#include "data/LogicGearBoostData.h"

#define skipDataReferenceArray for (int iterator = stream->readVInt();iterator > 0;iterator--) {\
            stream->readDataReference();\
        }

class LogicHeroUpgrades {
public:
    int level;
    LogicCardData* starpower;
    LogicCardData* gadget;
    LogicGearBoostData* gear1;
    LogicGearBoostData* gear2;
    LogicCardData* hypercharge;

    LogicHeroUpgrades() {
        level = 0;
        starpower = nullptr;
        gadget = nullptr;
        gear1 = nullptr;
        gear2 = nullptr;
        hypercharge = nullptr;
    }

    LogicHeroUpgrades(ByteStream* stream) {
        level = stream->readVInt();
        starpower = (LogicCardData*)stream->readDataReference();
        gadget = (LogicCardData*)stream->readDataReference();
        gear1 = (LogicGearBoostData*)stream->readDataReference();
        gear2 = (LogicGearBoostData*)stream->readDataReference();
        hypercharge = (LogicCardData*)stream->readDataReference();

        stream->readDataReference();
        stream->readDataReference();
        stream->readDataReference();
        skipDataReferenceArray;
    }

    LogicHeroUpgrades* clone() {
        auto logicHeroUpgrades = new LogicHeroUpgrades();
        logicHeroUpgrades->level = level;
        logicHeroUpgrades->starpower = starpower;
        logicHeroUpgrades->gadget = gadget;
        logicHeroUpgrades->gear1 = gear1;
        logicHeroUpgrades->gear2 = gear2;
        logicHeroUpgrades->hypercharge = hypercharge;
        return logicHeroUpgrades;
    }
};
#undef skipDataReferenceArray