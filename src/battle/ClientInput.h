#pragma once
#include "network/BitStream.h"
class ClientInput {
public:
    int type;
    int index;
    ClientInput(int type) {
        this->type = type;
    }
    ClientInput(ClientInput* input) {
        this->type = input->type;
        this->index = input->index;
    }
    void encode(BitStream* stream) {
        stream->writePositiveIntMax32767(index);
        stream->writePositiveIntMax31(type);
        stream->writePositiveIntMax32767(0);
        stream->writePositiveIntMax32767(0);
        stream->writeBoolean(false);
        stream->writeBoolean(true);
        stream->writeBoolean(false);

        stream->writeBoolean(false);
    }
};