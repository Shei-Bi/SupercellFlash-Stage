#pragma once
#include "LogicGameObjectClient.h"
#include "LogicBattleModeClient.h"
#include "data/LogicProjectileData.h"

class LogicProjectileClient :public LogicGameObjectClient {
public:
    LogicProjectileClient(LogicData* data) : LogicGameObjectClient(data) {
        ;
    }
    ~LogicProjectileClient() {
        ;
    }
    int getType() {
        return 1;
    }
};