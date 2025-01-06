#pragma once
#include <network/PiranhaMessage.h>

class MatchmakeRequestMessage : public PiranhaMessage {
    int id;
    int slot;
public:

    MatchmakeRequestMessage(int eventId, int eventSlot) :id(eventId), slot(eventSlot) {
        ;
    }
    ~MatchmakeRequestMessage() {
        ;
    }
    void encode() override {
        stream->writeVInt(0);//anticheat
        // stream->writeDataReference();
        stream->writeVInt(16);
        stream->writeVInt(0);

        stream->writeVInt(slot);//slot
        stream->writeVInt(3);//characters
        stream->writeVInt(16);
        stream->writeVInt(0);
        stream->writeVInt(16);
        stream->writeVInt(0);
        stream->writeVInt(16);
        stream->writeVInt(0);
        stream->writeVInt(id);//id
    }
    short getMessageType() override {
        return 18977;
    }
};