#pragma once
#include <network/PiranhaMessage.h>
#include <algorithm>
#include "network/BitStream.h"
#include "battle/ClientInput.h"
#include <vector>

class ClientInputMessage : public PiranhaMessage {
    std::vector<ClientInput*> inputs;
public:
    int tickCounter;
    int milisecondsSinceLastInput;
    int index;
    ~ClientInputMessage() {
        for (auto i : inputs) delete i;
    }
    void encode() override {
        BitStream bitStream(58);
        bitStream.writePositiveIntMax16383(std::clamp(tickCounter, 0, 16383));
        bitStream.writePositiveIntMax1023(std::clamp(milisecondsSinceLastInput, 0, 1023));
        bitStream.writePositiveIntMax8191(std::clamp(index, 0, 8191));
        bitStream.writePositiveIntMax1023(0);
        bitStream.writePositiveIntMax1023(0);
        bitStream.writePositiveIntMax1023(1);
        bitStream.writePositiveIntMax31(inputs.size());
        for (auto i : inputs)
            ;
        stream->writeBytesWithoutLength(bitStream.getByteArray(), bitStream.getLength());
    }
    void addInput(ClientInput* input) {
        inputs.push_back(input);
    }
    short getMessageType() override {
        return 10555;
    }
};