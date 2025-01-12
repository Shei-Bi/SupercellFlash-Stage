#pragma once
#include <network/PiranhaMessage.h>
#include <logic/LogicPlayer.hpp>
#include <vector>
#include "data/LogicLocationData.h"

class StartLoadingMessage : public PiranhaMessage {
public:
    int ownPlayerIndex;
    int ownTeamIndex;
    int gameMode;
    int mapType;
    int gameModeVariation;
    LogicLocationData* location;
    std::vector<LogicPlayer*> players;
    std::vector<int> modifiers;

    StartLoadingMessage() {
        ;
    }
    ~StartLoadingMessage() {
        for (auto i : players) delete i;
    }
    void decode() override {
        stream->readInt();
        ownPlayerIndex = stream->readInt();
        ownTeamIndex = stream->readInt();
        int playerSize = stream->readInt();
        players.reserve(playerSize);
        for (int i = 0;i < playerSize;i++) players.push_back(new LogicPlayer(stream));
        if (stream->readInt() > 0) abort(); //unused array
        int modifierSize = stream->readInt();
        modifiers.reserve(modifierSize);
        for (int i = 0;i < modifierSize;i++) modifiers.push_back(stream->readInt());
        stream->readInt();
        gameMode = stream->readVInt();
        mapType = stream->readVInt();
        gameModeVariation = stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        location = (LogicLocationData*)stream->readDataReference();
        if (stream->readBoolean()) abort();
    }
    short getMessageType() override {
        return 20559;
    }
};