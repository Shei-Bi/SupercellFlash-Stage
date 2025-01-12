#include "network/VisionUpdateMessage.h"
#include <vector>
#include <algorithm>
#include "LogicGameObjectManagerClient.h"
#include "data/LogicLocationData.h"
#include "LogicTileMap.h"

class LogicBattleModeClient {
public:
    std::vector<LogicPlayer*> players;
    bool startLoadingReceived;
    bool visionUpdateReceived;
    int lastHandledTick;
    int droppedVUs;
    std::vector<VisionUpdateMessage*> cachedVisions;
    LogicGameObjectManagerClient* nextTickObjects;
    LogicGameObjectManagerClient* thisTickObjects;
    LogicGameObjectManagerClient* objectManager;
    LogicTileMap* tileMap;
    int gameModeVariation;
    LogicLocationData* location;
    int ownPlayerIndex;
    int ownTeamIndex;

    LogicBattleModeClient() {
        startLoadingReceived = false;
        visionUpdateReceived = false;
        lastHandledTick = -1;
        droppedVUs = 0;
        objectManager = new LogicGameObjectManagerClient();
        nextTickObjects = new LogicGameObjectManagerClient();
        thisTickObjects = new LogicGameObjectManagerClient();
        gameModeVariation = 0;
        location = nullptr;
        tileMap = new LogicTileMap();
    }

    void setGameModeVariationData(int v) {
        gameModeVariation = v;
    }
    void setLocation(LogicLocationData* l) {
        location = l;
    }
    void generateTileMap() {
        tileMap->generateTileMap(location);
    }
    void setPlayerCount(int count) {
        players.reserve(count);
    }
    void addPlayer(LogicPlayer* player) {
        players.push_back(player);
    }
    bool isStartLoadingMessageReceived() {
        return startLoadingReceived;
    }
    void update(float sinceStart, float deltaTime) {
        if (cachedVisions.size() < 3) return;
        objectManager->decode(cachedVisions.back()->visionBitStream, players, false, this, nullptr);
        cachedVisions.back()->visionBitStream->resetOffset();
    }
    bool canProceedToBattle() {
        return visionUpdateReceived;
    }
    bool addVisionUpdate(VisionUpdateMessage* message) {
        int serverTime = message->tickCounter;
        int lastVisionTime = -1;
        // printf("incoming: %d\n", serverTime);
        // printf("in queue:  ");
        // for (auto i : cachedVisions) printf("%d  ", i->tickCounter);
        // printf("\ndropped vus: %d\n", droppedVUs);
        if (cachedVisions.size() == 0) goto cache;
        lastVisionTime = cachedVisions.back()->tickCounter;
        if (std::abs(lastVisionTime - serverTime) >= 20) {
            for (auto i : cachedVisions) delete i;
            cachedVisions.clear();
        }
        else if (lastVisionTime > serverTime) return false;
        else if (cachedVisions.size() >= 5) {
            delete cachedVisions.front();
            cachedVisions.erase(cachedVisions.begin());
        }
    cache:
        if (cachedVisions.size() == 0) lastHandledTick = serverTime - 1;
        cachedVisions.push_back(message);
        if (serverTime != lastHandledTick + 1) droppedVUs += (serverTime - lastHandledTick - 1);
        lastHandledTick = serverTime;
        if (cachedVisions.size() >= 3) visionUpdateReceived = true;
        return true;
    }
};