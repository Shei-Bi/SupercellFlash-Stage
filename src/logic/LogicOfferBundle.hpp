#include "LogicGemOffer.hpp"
#include "ChronosTextEntry.hpp"
#include <vector>
class LogicOfferBundle {
    std::vector<LogicGemOffer*> offers;
    int currencyType;
    int price;
    int timeLeft;
    const char* bgr;
    ChronosTextEntry* title;
    ChronosTextEntry* title2;
public:
    LogicOfferBundle(ByteStream* stream) {
        int offerSize = stream->readVInt();
        offers.reserve(offerSize);
        for (int i = 0;i < offerSize;i++) {
            offers.push_back(new LogicGemOffer(stream));
        }
        currencyType = stream->readVInt();
        price = stream->readVInt();
        timeLeft = stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();//
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        title = new ChronosTextEntry(stream);
        stream->readBoolean();
        bgr = stream->readString(100);
        stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        const char* idk = stream->readString(9000);
        if (idk) delete[] idk;
        stream->readBoolean();
        stream->readBoolean();
        stream->readDataReference();
        stream->readDataReference();
        stream->readDataReference();//
        stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        stream->readVInt();
    }
};