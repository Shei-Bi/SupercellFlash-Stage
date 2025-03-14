#pragma once
#include <network/PiranhaMessage.h>
#include <network/ServerHelloMessage.h>
#include <network/LoginOkMessage.h>
#include <network/OwnHomeDataMessage.hpp>
#include <network/UdpConnectionInfoMessage.h>
#include <network/StartLoadingMessage.h>
#include <network/VisionUpdateMessage.h>
#include "network/BattleEndMessage.h"

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
        case 24112:
            return new UdpConnectionInfoMessage();
        case 20559:
            return new StartLoadingMessage();
        case 24109:
            return new VisionUpdateMessage();
        case 23456:
            return new BattleEndMessage();
        }
        return nullptr;
    }
};