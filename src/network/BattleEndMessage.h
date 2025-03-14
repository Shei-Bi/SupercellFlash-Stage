#pragma once
#include <network/PiranhaMessage.h>

class BattleEndMessage : public PiranhaMessage {
public:
    int result;
    void decode() override {
        stream->readLongLong();
        stream->readLongLong();
        stream->readVInt();
        result = stream->readVInt();
    }
    short getMessageType() override {
        return 23456;
    }
};