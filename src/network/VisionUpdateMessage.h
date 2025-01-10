#pragma once
#include <network/PiranhaMessage.h>
#include "network/BitStream.h"
#include "assert.h"

class VisionUpdateMessage : public PiranhaMessage {
public:
    int tickCounter;
    int handledInputs;

    int spectatorCount;
    bool isBrawlTVEnabled;

    BitStream* visionBitStream;

    VisionUpdateMessage() {
        visionBitStream = nullptr;
    }
    ~VisionUpdateMessage() {
        if (visionBitStream) delete visionBitStream;
    }
    void decode() override {
        assert(false);
    }
    bool decodeUDP(int length) {
        int offset1 = stream->getOffset();
        tickCounter = stream->readVInt();
        handledInputs = stream->readVInt();
        stream->readVInt();
        spectatorCount = stream->readVInt();
        isBrawlTVEnabled = stream->readBoolean();
        if (stream->readBoolean()) {
            stream->readVInt();
        }
        if (stream->readBoolean()) {
            stream->readVInt();
            stream->readVInt();
        }
        if (visionBitStream) {
            delete visionBitStream;
            visionBitStream = nullptr;
        }
        int visionBitStreamLength = stream->readInt();
        int offset2 = stream->getOffset();
        if (visionBitStreamLength != length - (offset2 - offset1)) return false;
        visionBitStream = new BitStream((unsigned char*)stream->readBytes(visionBitStreamLength, 900000), visionBitStreamLength);
        return true;
    }
    short getMessageType() override {
        return 24109;
    }
};