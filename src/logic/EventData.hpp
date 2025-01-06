#pragma once
#include "network/ByteStream.h"
#include "data/LogicLocationData.h"
#include "ChronosTextEntry.hpp"

#define skipIntArray for (int iterator = stream->readVInt();iterator > 0;iterator--) {\
            stream->readVInt();\
        }
#define skipDataReferenceArray for (int iterator = stream->readVInt();iterator > 0;iterator--) {\
            stream->readDataReference();\
        }
#define skipLogicGemOffer stream->readVInt();\
                        stream->readVInt();\
                        stream->readDataReference();\
                        stream->readVInt();
class EventData {
public:
    int id;
    int slot;
    LogicLocationData* location;
    ChronosTextEntry* text1;
    ChronosTextEntry* text2;
    ChronosTextEntry* text3;
    EventData(ByteStream* stream) {
        id = stream->readVInt();
        slot = stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        location = (LogicLocationData*)stream->readDataReference();
        stream->readVInt();
        stream->readVInt();
        delete[] stream->readString(9000);
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        skipIntArray;
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        bool mm = stream->readBoolean();
        assert(!mm);
        stream->readVInt();
        if (stream->readBoolean()) {
            stream->readVInt();
            delete[] stream->readString(9000);
            stream->readVInt();
            stream->readVInt();
            for (int i = stream->readVInt();i > 0;i--) {
                if (stream->readBoolean()) {
                    stream->readVInt();
                    stream->readVInt();
                }
                if (stream->readBoolean()) skipLogicGemOffer;
            }
            for (int i = stream->readVInt();i > 0;i--) {
                stream->readVInt();
                stream->readVInt();
            }
            for (int i = stream->readVInt();i > 0;i--) {
                stream->readVInt();
                stream->readVInt();
            }
        }
        stream->readVInt();
        stream->readVInt();
        if (stream->readBoolean()) text1 = new ChronosTextEntry(stream);
        else text1 = nullptr;
        if (stream->readBoolean()) text2 = new ChronosTextEntry(stream);
        else text2 = nullptr;
        if (stream->readBoolean()) text3 = new ChronosTextEntry(stream);
        else text3 = nullptr;
        if (stream->readBoolean()) { skipLogicGemOffer };
        skipIntArray;
        bool ChronosFileEntry = stream->readBoolean();
        assert(!ChronosFileEntry);
        if (stream->readBoolean()) {
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
        }
        skipDataReferenceArray;
        skipDataReferenceArray;
        skipIntArray;
        stream->readVInt();
        bool idk = stream->readBoolean();
        assert(!idk);
        idk = stream->readBoolean();
        assert(!idk);
        idk = stream->readBoolean();
        assert(!idk);
        idk = stream->readBoolean();
        assert(!idk);
    }
};
#undef skipIntArray
#undef skipDataReferenceArray
#undef skipLogicGemOffer