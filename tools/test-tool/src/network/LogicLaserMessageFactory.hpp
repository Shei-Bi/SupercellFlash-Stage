#pragma once
#include <network/PiranhaMessage.h>
#include <network/ServerHelloMessage.h>

class LogicLaserMessageFactory {
public:
    static PiranhaMessage* createMessageByType(short type) {
        switch (type) {
        case 20100:
            return new ServerHelloMessage();
        }
        return nullptr;
    }
};