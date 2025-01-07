#include "network/ByteStream.h"
#include <assert.h>

class IntValueEntry {
    int shei;
    int bi;
public:
    int& operator [](int index) {
        assert(index == 0 || index == 1);
        return index == 0 ? shei : bi;
    }
    IntValueEntry(ByteStream* stream) {
        operator[](0) = stream->readVInt();
        operator[](1) = stream->readVInt();
    }
};