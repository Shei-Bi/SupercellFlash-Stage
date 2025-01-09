#pragma once
#include <network/PiranhaMessage.h>

class StartLoadingMessage : public PiranhaMessage {
public:
    int port;
    const char* addr;
    unsigned char* sessionId;
    int sessionIdLength;
    unsigned char* nonce;
    int nonceLength;

    StartLoadingMessage() {
        addr = nullptr;
        sessionId = nullptr;
        nonce = nullptr;
    }
    ~StartLoadingMessage() {
        ;
    }
    void decode() override {
        port = stream->readVInt();
        addr = stream->readString(900000);
        sessionIdLength = stream->readInt();
        sessionId = (unsigned char*)stream->readBytes(sessionIdLength, 900000);
        nonceLength = stream->readInt();
        nonce = (unsigned char*)stream->readBytes(nonceLength, 900000);
    }
    short getMessageType() override {
        return 20559;
    }
};