#pragma once
#include "LogicVector2.h"
#include "data/LogicData.h"
#include "network/BitStream.h"
#include "data/LogicCharacterData.h"
#include "data/LogicProjectileData.h"

class LogicGameObjectClient {
public:
    int globalID;
    LogicData* data;
    LogicVector2 position;
    int z;
    int fadeCounter;
    int playerIndex;
    int teamIndex;
    int worldIndex;

    LogicGameObjectClient(LogicData* data) :data(data) {
        ;
    }
    virtual ~LogicGameObjectClient() {
        ;
    }
    virtual void decode(BitStream* stream, int gmv, bool isPlayedOnVeryLargeMap) {
        if (false) {
            //todo: spray
            abort();
        }
        else {
            if ((getType() == 0 && ((LogicCharacterData*)getData())->type == Train) ||
                (getType() == 1 && ((LogicProjectileData*)getType())->ignoreLevelBoarder)) {
                position.x = stream->readIntMax65535();
                position.y = stream->readIntMax65535();
            }
            else {
                position.x = stream->readPositiveVIntMax65535();
                position.y = stream->readPositiveVIntMax65535();
            }
            z = stream->readPositiveVIntMax65535();
        }
        int index = stream->readPositiveVIntMax255();
        int playerCount = LogicDataTables::getGameModeVariationData(gmv)->getPlayerCount();
        playerIndex = index & 15;
        if (playerIndex == playerCount) playerIndex = -1;
        teamIndex = index / 16;
        if (teamIndex == playerCount) teamIndex = -1;
        worldIndex = stream->readPositiveIntMax3();
        if (getType() != 1) fadeCounter = stream->readPositiveIntMax15();
    }
    int getGlobalID() {
        return globalID;
    }
    LogicData* getData() {
        return data;
    }
    int getX() {
        return position.x;
    }
    int getY() {
        return position.y;
    }
    virtual int getType() = 0;
};