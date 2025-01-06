#pragma once
#include "network/ByteStream.h"
class ChronosTextEntry {
    int type;
    const char* text;
public:
    ChronosTextEntry(ByteStream* stream) {
        text = stream->readString(900000);
        type = stream->readVInt();
    }
};