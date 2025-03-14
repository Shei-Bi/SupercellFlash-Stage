#pragma once
#include <network/PiranhaMessage.h>

class GoHomeMessage : public PiranhaMessage {
public:
    void encode() override {
        stream->writeBoolean(false);
        stream->writeBoolean(false);
        stream->writeBoolean(false);
    }
    short getMessageType() override {
        return 14456;
    }
};