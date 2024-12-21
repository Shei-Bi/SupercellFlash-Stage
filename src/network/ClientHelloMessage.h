#pragma once
#include <network/PiranhaMessage.h>

class ClientHelloMessage : public PiranhaMessage {
public:
    int protocol;
    int keyVersion;
    int clientMajor;
    int clientMinor;
    int clientBuild;
    std::string* fingerprintSha;
    int deviceType;
    int appStore;

    ClientHelloMessage();
    ~ClientHelloMessage();
    void encode() override;
    short getMessageType() override;
};