#pragma once
#include <network/PiranhaMessage.h>
#include <string>
#include <logic/LogicClientAvatar.hpp>
#include <logic/LogicClientHome.hpp>

class OwnHomeDataMessage : public PiranhaMessage {
public:
    LogicClientHome* home;
    LogicClientAvatar* avatar;
    void decode() override {
        home = new LogicClientHome();
        home->decode(stream);
        avatar = new LogicClientAvatar();
        avatar->decode(stream);
    };
    short getMessageType() override {
        return 24101;
    };
};