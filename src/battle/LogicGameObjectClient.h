#pragma once
#include "LogicVector2.h"
#include "data/LogicData.h"
#include "network/BitStream.h"

class LogicGameObjectClient {
    int globalID;
    LogicData* data;
    LogicVector2 position;
    int z;
    int fadeCounter;
public:
    int playerIndex;
    int teamIndex;
    int worldIndex;

    LogicGameObjectClient(LogicData* data) :data(data) {
        ;
    }
    virtual ~LogicGameObjectClient() {
        ;
    }
    virtual void decode() {
        ;
    }
};