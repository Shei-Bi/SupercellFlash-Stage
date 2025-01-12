#pragma once
#include "LogicHeroUpgrades.hpp"
#include "data/LogicCharacterData.h"
#include "network/ByteStream.h"
#include "data/LogicSkinData.h"

#define skipDataReferenceArray for (int iterator = stream->readVInt();iterator > 0;iterator--) {\
            stream->readDataReference();\
        }

class LogicHeroEntry {
public:
    LogicCharacterData* character;
    LogicHeroUpgrades* upgrades;
    LogicSkinData* skin;
    LogicHeroEntry() {
        ;
    }
    ~LogicHeroEntry() {
        delete upgrades;
    }
    LogicHeroEntry(ByteStream* stream) {
        upgrades = nullptr;
        character = (LogicCharacterData*)stream->readDataReference();
        if (stream->readBoolean()) {
            upgrades = new LogicHeroUpgrades(stream);
        }
        if (stream->readBoolean()) {
            skipDataReferenceArray;
            stream->readVInt();
        }
        if (stream->readBoolean()) {
            skipDataReferenceArray;
        }
        skin = (LogicSkinData*)stream->readDataReference();
        stream->readDataReference();
        stream->readVInt();
    }
    LogicHeroEntry* clone() {
        auto logicHeroEntry = new LogicHeroEntry();
        logicHeroEntry->character = character;
        logicHeroEntry->upgrades = upgrades->clone();
        logicHeroEntry->skin = skin;
        return logicHeroEntry;
    }
};
#undef skipDataReferenceArray