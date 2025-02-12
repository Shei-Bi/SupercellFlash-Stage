#pragma once
#include <network/PiranhaMessage.h>

class KeepAliveMessage : public PiranhaMessage {
public:
    void encode() override {
        ;
    }
    short getMessageType() override {
        return 10108;
    }
};