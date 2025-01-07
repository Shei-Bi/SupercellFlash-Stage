#pragma once
#include <network/PiranhaMessage.h>

class UdpConnectionInfoMessage : public PiranhaMessage {
public:
    int port;
    const char* addr;
    unsigned char* sessionId;
    int sessionIdLength;
    unsigned char* nonce;
    int nonceLength;

    UdpConnectionInfoMessage() {
        addr = nullptr;
        sessionId = nullptr;
        nonce = nullptr;
    }
    ~UdpConnectionInfoMessage() {
        if (addr) delete[] addr;
        if (sessionId) delete[] sessionId;
        if (nonce) delete[] nonce;
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
        return 24112;
    }
};