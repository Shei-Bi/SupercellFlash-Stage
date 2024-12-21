#pragma once
#include <network/PiranhaMessage.h>

class LoginMessage : public PiranhaMessage {
public:
    long long accountId;
    std::string* token;
    int clientMajor;
    int clientMinor;
    int clientBuild;
    std::string* fingerprintSha;

    LoginMessage();
    ~LoginMessage();
    void encode() override;
    short getMessageType() override;
};