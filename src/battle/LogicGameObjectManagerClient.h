#pragma once
#include "network/BitStream.h"
#include "stdio.h"
#include "logic/LogicPlayer.hpp"
#include <vector>
class LogicBattleModeClient;
class LogicGameObjectClient;

class LogicGameObjectManagerClient {
public:
    std::vector<LogicGameObjectClient*> gameObjects;
    int ownObjectID;
    int willowObjectID;
    int gemGrabCountDown;
    int roundState;
    void decode(BitStream* stream, std::vector<LogicPlayer*>& players, bool overwriteCurrentState, LogicBattleModeClient* battleClient, std::vector<LogicPlayer*>* interpolatedProjectiles);
    void debugLog();
};