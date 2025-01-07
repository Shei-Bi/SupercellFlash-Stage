#pragma once
#include <network/PiranhaMessage.h>

class SinglePlayerMatchRequestMessage : public PiranhaMessage {
    int gameType;
    LogicData* character;
    LogicData* skin;
    bool wtf;
    LogicData* location;
public:

    SinglePlayerMatchRequestMessage(int gameType, LogicData* chaarcter, LogicData* skin, bool wtf, LogicData* location) :gameType(gameType), character(chaarcter), skin(skin), wtf(wtf), location(location) {
        ;
    }
    ~SinglePlayerMatchRequestMessage() {
        ;
    }
    void encode() override {
        stream->writeDataReference(character);
        stream->writeDataReference(skin);
        stream->writeVInt(gameType);
        stream->writeBoolean(wtf);
        stream->writeDataReference(location);
    }
    short getMessageType() override {
        return 14118;
    }
};