#include "network/ByteStream.h"
// #define log(x) (printf("%d\n",x))
#define log(x) (x)
class BrawlPassSeasonData {
    int season;
    int tokensGained;
public:
    BrawlPassSeasonData(ByteStream* stream) {
        season = stream->readVInt();
        tokensGained = stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        stream->readBoolean();
        if (stream->readBoolean()) {
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
        }
        if (stream->readBoolean()) {
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
        }
        stream->readBoolean();//??????????
        if (stream->readBoolean()) {
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
            log(stream->readInt());
        }
    }
};
#undef log