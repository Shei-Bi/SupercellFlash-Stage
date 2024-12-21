#pragma once
#include <network/PiranhaMessage.h>
#include <string>

class LoginOkMessage : public PiranhaMessage {
public:
    long long accountId;
    long long homeId;
    std::string* token;
    LoginOkMessage();
    ~LoginOkMessage();
    void decode() override;
    short getMessageType() override;
};