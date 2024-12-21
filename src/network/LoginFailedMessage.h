#pragma once
#include <network/PiranhaMessage.h>
#include <string>

class LoginFailedMessage : public PiranhaMessage {
private:
    std::string resourceSha;
public:
    LoginFailedMessage();
    ~LoginFailedMessage();
    void decode() override;
    short getMessageType() override;
};