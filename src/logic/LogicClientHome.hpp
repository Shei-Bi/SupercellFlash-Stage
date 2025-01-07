#pragma once
#include <network/ByteStream.h>
#include "LogicOfferBundle.hpp"
#include "IntValueEntry.hpp"
#include "CooldownEntry.hpp"
#include "BrawlPassSeasonData.hpp"
#include "EventData.hpp"
#include <assert.h>

#define log(x) (printf("%d\n",x))
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

class LogicClientHome {
public:
    int currentTrophies;
    int highestTrophies;
    int highestTrophiesToday;
    LogicData* thumbnail;
    LogicData* namecolor;
    std::vector<LogicOfferBundle*> offerBundles;
    std::vector<LogicData*> characters;
    const char* currentRegion;
    const char* supportedCC;
    std::vector<IntValueEntry*> intValues;
    std::vector<CooldownEntry*> cooldowns;
    std::vector<BrawlPassSeasonData*> bpSeasons;
    std::vector<EventData*> events;
    std::vector<EventData*> upcomingEvents;

    EventData* getFirstEventOfGMV(int variadtion) {
        for (EventData* e : events) if (e->location->getGameModeVariation() == variadtion) return e;
        return nullptr;
    }

    EventData* getActiveEventForSlot(int slot) {
        for (EventData* e : events) if (e->slot == slot) return e;
        return nullptr;
    }
    void decode(ByteStream* stream) {
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        currentTrophies = stream->readVInt();
        highestTrophies = stream->readVInt();
        highestTrophiesToday = stream->readVInt();
        log(stream->readVInt());
        log(stream->readVInt());
        thumbnail = stream->readDataReference();
        namecolor = stream->readDataReference();
        skipIntArray;
        skipDataReferenceArray;
        skipDataReferenceArray;
        skipDataReferenceArray;
        skipDataReferenceArray;
        skipDataReferenceArray;
        skipDataReferenceArray;

        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readBoolean();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        stream->readVInt();
        {
            stream->readVInt();
            stream->readVInt();
            skipIntArray;
        }
        log(stream->readBoolean());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        int offerBundleSize = stream->readVInt();
        offerBundles.reserve(offerBundleSize);
        for (int i = 0;i < offerBundleSize;i++) {
            offerBundles.push_back(new LogicOfferBundle(stream));
        }
        log(stream->readVInt());
        log(stream->readVInt());
        int AdStatus = stream->readVInt();//AdStatus: always 0.
        assert(AdStatus == 0);
        log(stream->readVInt());
        log(stream->readVInt());
        int characterSize = stream->readByte();
        // assert(characterSize == 3);
        characters.reserve(characterSize);
        for (int i = 0;i < characterSize;i++) characters.push_back(stream->readDataReference());
        currentRegion = stream->readString(120);
        supportedCC = stream->readString(120);
        int intValueSize = stream->readVInt();
        intValues.reserve(intValueSize);
        for (int i = 0;i < intValueSize;i++) {
            intValues.push_back(new IntValueEntry(stream));
        }
        int cooldownSize = stream->readVInt();
        cooldowns.reserve(cooldownSize);
        for (int i = 0;i < cooldownSize;i++) {
            cooldowns.push_back(new CooldownEntry(stream));
        }
        int bpSeasonSize = stream->readVInt();
        bpSeasons.reserve(bpSeasonSize);
        for (int i = 0;i < bpSeasonSize;i++) {
            bpSeasons.push_back(new BrawlPassSeasonData(stream));
        }
        if (stream->readBoolean()) {//LogicQuests
            for (int i = stream->readVInt();i > 0;i--) {
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                skipDataReferenceArray;
                skipIntArray;
                skipIntArray;
                stream->readVInt();
                stream->readBoolean();
                stream->readBoolean();

                skipLogicGemOffer;
                if (stream->readBoolean()) skipLogicGemOffer;
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readBoolean();
                stream->readBoolean();
                stream->readBoolean();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
            }
            stream->readVInt();
            stream->readVInt();
            for (int i = stream->readVInt();i > 0;i--) {
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                stream->readVInt();
                skipDataReferenceArray;
                skipIntArray;
                skipIntArray;
                stream->readVInt();
                stream->readBoolean();
                stream->readBoolean();

                stream->readVLong();
                stream->readVInt();
            }
        }
        if (stream->readBoolean()) {//VanityItems
            for (int i = stream->readVInt();i > 0;i--) {
                stream->readDataReference();
                for (int j = stream->readVInt();j > 0;j--) {
                    stream->readVInt();
                    stream->readVInt();
                }
            }
        }
        if (stream->readBoolean()) {//LogicPlayerRankedSeasonData
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            stream->readVInt();
            for (int i = stream->readVInt();i > 0;i--) {
                if (stream->readBoolean()) {
                    if (stream->readBoolean()) {
                        stream->readVInt();
                        stream->readVInt();
                    }
                    if (stream->readBoolean()) skipLogicGemOffer;
                }
                stream->readVInt();
                stream->readBoolean();
            }
        }
        log(stream->readInt());
        log(stream->readVInt());
        stream->readDataReference();
        if (stream->readBoolean()) {
            for (int i = stream->readVInt();i > 0;i--) { skipLogicGemOffer };
        }
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());
        log(stream->readVInt());

        log(stream->readVInt());
        skipIntArray;//played events
        int eventSize = stream->readVInt();
        events.reserve(eventSize);
        for (int i = 0;i < eventSize;i++) events.push_back(new EventData(stream));
        int upcomingEventSize = stream->readVInt();
        upcomingEvents.reserve(upcomingEventSize);
        for (int i = 0;i < upcomingEventSize;i++) upcomingEvents.push_back(new EventData(stream));
    }
};
#undef log
#undef skipIntArray
#undef skipDataReferenceArray
#undef skipLogicGemOffer