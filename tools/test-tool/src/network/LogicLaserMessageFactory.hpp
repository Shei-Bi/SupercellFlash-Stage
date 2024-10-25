#pragma once
#include <network/PiranhaMessage.h>
#include <network/ServerHelloMessage.h>
#include <network/LoginOkMessage.h>
#include <network/OwnHomeDataMessage.hpp>

class LogicLaserMessageFactory {
public:
    static PiranhaMessage* createMessageByType(short type) {
        switch (type) {
        case 20100:
            return new ServerHelloMessage();
        case 20104:
            return new LoginOkMessage();
        case 24101:
            return new OwnHomeDataMessage();
        }
        return nullptr;
    }
};