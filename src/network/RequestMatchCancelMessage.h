#pragma once
#include <network/PiranhaMessage.h>

class RequestMatchCancelMessage : public PiranhaMessage {
public:
    void encode() override {
        ;
    }
    short getMessageType() override {
        return 14117;
    }
};