#pragma once
#include <network/PiranhaMessage.h>
#include "data/LogicCharacterData.h"

class MatchmakeRequestMessage : public PiranhaMessage {
    int id;
    int slot;
    LogicData* character;
    std::vector<LogicCharacterData*> characters;
public:

    MatchmakeRequestMessage(LogicCharacterData* character, std::vector<LogicCharacterData*>& characters, int eventId, int eventSlot) :id(eventId), slot(eventSlot), character(character), characters(characters) {
        ;
    }
    ~MatchmakeRequestMessage() {
        ;
    }
    void encode() override {
        stream->writeVInt(0);//anticheat
        stream->writeDataReference(character);

        stream->writeVInt(slot);//slot
        stream->writeVInt(characters.size());
        for (auto i : characters) stream->writeDataReference(i);

        stream->writeVInt(id);//id
    }
    short getMessageType() override {
        return 18977;
    }
};