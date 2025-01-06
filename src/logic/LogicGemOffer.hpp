#include "network/ByteStream.h"
class LogicGemOffer {
public:
    int type;
    int quantity;
    LogicData* data;
    int extraData;
    LogicGemOffer(ByteStream* stream) {
        type = stream->readVInt();
        quantity = stream->readVInt();
        data = stream->readDataReference();
        extraData = stream->readVInt();
    }
};