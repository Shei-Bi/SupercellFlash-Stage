#pragma once
#include <network/PiranhaMessage.h>

class ClientInfoMessage : public PiranhaMessage {
public:
    void encode() override {
        stream->writeString("eth0");
    }
    short getMessageType() override {
        return 10177;
    }
};