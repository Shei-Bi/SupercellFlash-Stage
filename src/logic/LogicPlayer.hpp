#pragma once
#include "network/ByteStream.h"
#include "LogicHeroEntry.hpp"
#include <vector>
#include "PlayerDisplayData.hpp"
#include "battle/LogicAccessory.h"

class LogicPlayer {
public:
    long long accountId;
    int playerIndex;
    int teamIndex;
    std::vector<LogicHeroEntry*> heroes;
    PlayerDisplayData* displayData;

    int ultiCharge;
    int ocCharge;
    int heroIndex;
    LogicAccessory* accessory;
    int accessoryCharges;

    int objectGlobalID;

    LogicPlayer() {
        accountId = 0;
        playerIndex = 0;
        teamIndex = 0;
        displayData = nullptr;
        ultiCharge = 0;
        ocCharge = 0;
        heroIndex = 0;
        accessory = nullptr;
        accessoryCharges = 0;
        objectGlobalID = 0;
    }
    void setHeroIndex(int index) {
        heroIndex = index;

        if (accessory) delete accessory;
        accessory = new LogicAccessory(heroes[heroIndex]->upgrades->gadget->getAccessory());
    }
    bool hasOverchargeData() {
        auto upgrades = heroes[heroIndex]->upgrades;
        return upgrades != nullptr && upgrades->hypercharge != nullptr;
    }
    ~LogicPlayer() {
        for (auto i : heroes) delete i;
        delete displayData;
    }
    LogicPlayer(ByteStream* stream) {
        displayData = nullptr;

        accountId = stream->readLongLong();
        if (stream->readBoolean()) {
            displayData = new PlayerDisplayData(stream);
            stream->readVInt();
            stream->readDataReference();
            stream->readDataReference();
            stream->readDataReference();
            stream->readDataReference();
            stream->readVInt();
            stream->readVInt();
        }
        playerIndex = stream->readVInt();
        teamIndex = stream->readVInt();
        stream->readVInt();
        stream->readInt();
        int heroCount = stream->readInt8();
        heroes.reserve(heroCount);
        for (int i = 0;i < heroCount;i++) heroes.push_back(new LogicHeroEntry(stream));
        stream->readBoolean();
        if (stream->readBoolean()) {
            stream->readVLong();
            delete[] stream->readString(9000);
            stream->readDataReference();
        }
        stream->readInt8();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
    }

    LogicPlayer* clone() {
        auto logicPlayer = new LogicPlayer();
        logicPlayer->accountId = accountId;
        logicPlayer->playerIndex = playerIndex;
        logicPlayer->teamIndex = teamIndex;
        logicPlayer->heroes.reserve(heroes.size());
        for (auto i : heroes) logicPlayer->heroes.push_back(i->clone());
        logicPlayer->displayData = displayData->clone();

        logicPlayer->setHeroIndex(0);
        return logicPlayer;
    }
};