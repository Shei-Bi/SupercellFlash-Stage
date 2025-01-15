#pragma once
#include "data/LogicAccessoryData.h"
#include "network/BitStream.h"

class LogicAccessory {
public:
    int state;
    int cooldown;
    int activateTick;
    int activationAngle;
    LogicAccessoryData* data;
    LogicAccessory(LogicAccessoryData* data) :data(data) {
        ;
    }
    void decode(BitStream* stream, bool isOwn, bool overwriteCurrentState) {
        int idk = 0;
        int cooldown = 0;
        int state = 2;
        int activateTick = 0;
        int activationAngle = 0;
        if (isOwn) {
            if (stream->readBoolean())
                idk = stream->readPositiveVIntMax65535();
            cooldown = stream->readPositiveVIntMax65535();
            state = stream->readPositiveVIntMax255OftenZero();

            //todo: heal_damage_taken special case

            if (state == 1) {
                activateTick = stream->readPositiveIntMax16383();
                activationAngle = stream->readPositiveIntMax511();
            }
        }
        else {
            if (stream->readBoolean()) {
                state = 1;
                activateTick = stream->readPositiveIntMax16383();
                activationAngle = stream->readPositiveIntMax511();
            }
        }
        if (overwriteCurrentState) {
            this->state = state;
            this->cooldown = cooldown;
            this->activateTick = activateTick;
            this->activationAngle = activationAngle;
        }
    }
};