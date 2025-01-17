#pragma once

class LogicCharacterClient;
#include "data/LogicSkillData.h"
#include "network/BitStream.h"
#include "LogicBattleModeClient.h"

class LogicSkillClient {
public:
    LogicSkillData* data;
    int maxCharge;
    int charge;
    int activeTime;
    int maxActiveTime;
    bool onActivate;
    int cooldown;

    LogicSkillClient(LogicSkillData* data);
    // ~LogicSkillClient();

    void decode(BitStream* stream, bool isOwn, LogicCharacterClient* character, LogicBattleModeClient* battleClient);
};