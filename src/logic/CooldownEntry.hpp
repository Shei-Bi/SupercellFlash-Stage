#include "network/ByteStream.h"
class CooldownEntry {
public:
    int a;
    int b;
    LogicData* c;
    CooldownEntry(ByteStream* stream) {
        a = stream->readVInt();
        b = stream->readVInt();
        c = stream->readDataReference();
    }
};