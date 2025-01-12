#pragma once
#include "network/ByteStream.h"

class PlayerDisplayData {
public:
    const char* name;

    PlayerDisplayData() {
        ;
    }

    ~PlayerDisplayData() {
        delete[] name;
    }

    PlayerDisplayData(ByteStream* stream) {
        name = stream->readString(9000);
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
    }

    PlayerDisplayData* clone() {
        auto playerDisplayData = new PlayerDisplayData();
        playerDisplayData->name = new char[strlen(name) + 1];
        strcpy((char*)playerDisplayData->name, name);
        return playerDisplayData;
    }
};