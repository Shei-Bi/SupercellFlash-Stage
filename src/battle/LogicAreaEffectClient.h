#pragma once
#include "LogicGameObjectClient.h"
#include "LogicBattleModeClient.h"
#include "data/LogicAreaEffectData.h"

class LogicAreaEffectClient :public LogicGameObjectClient {
public:
    LogicAreaEffectClient(LogicData* data) : LogicGameObjectClient(data) {
        ;
    }
    ~LogicAreaEffectClient() {
        ;
    }
    int getType() {
        return 2;
    }
};