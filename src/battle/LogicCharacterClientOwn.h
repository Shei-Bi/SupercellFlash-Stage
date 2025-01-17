#pragma once
#include "LogicCharacterClient.h"
class LogicCharacterClientOwn :public LogicCharacterClient {
public:
    LogicCharacterClientOwn(LogicData* data) : LogicCharacterClient(data) {
        ;
    }
};