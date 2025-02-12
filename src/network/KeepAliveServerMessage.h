#pragma once
#include <network/PiranhaMessage.h>

class KeepAliveServerMessage : public PiranhaMessage {
public:
    void decode() override {
        ;
    }
    short getMessageType() override {
        return 20108;
    }
};