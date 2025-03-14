#include "LogicGameObjectManagerClient.h"
#include "LogicBattleModeClient.h"
#include "util/LogicGameModeUtil.hpp"
#include "LogicGameObjectClient.h"
#include "LogicCharacterClient.h"
#include "LogicCharacterClientOwn.h"
#include "LogicAreaEffectClient.h"
#include "LogicProjectileClient.h"
#include "Starrpark.hpp"

void LogicGameObjectManagerClient::decode(BitStream* stream, std::vector<LogicPlayer*>& players, bool overwriteCurrentState, LogicBattleModeClient* battleClient, std::vector<LogicPlayer*>* interpolatedProjectiles) {
    ownObjectID = stream->readPositiveIntMax2097151();
    willowObjectID = stream->readPositiveIntMax2097151();
    if (stream->readBoolean()) {//modifiers array
        for (int i = stream->readPositiveIntMax63();i > 0;i--) stream->readPositiveIntMax63();
    }
    if (stream->readBoolean()) stream->readPositiveIntMax255();
    if (LogicGameModeUtil::isGemGrab(battleClient->gameModeVariation)) {
        gemGrabCountDown = stream->readPositiveVIntMax65535();
    }
    else if (LogicGameModeUtil::isKingOfHill(battleClient->gameModeVariation)) {
        abort();//todo
    }
    else if (battleClient->gameModeVariation == 19) {
        abort();//ProtectKing
    }
    else if (LogicGameModeUtil::isKnockOut(battleClient->gameModeVariation) || battleClient->gameModeVariation == 24) {
        abort();//todo
    }
    bool idk = stream->readBoolean();
    int roundState = stream->readIntMax15();
    if (overwriteCurrentState) {
        if (battleClient->roundState == -1)
            battleClient->roundState = roundState;
    }
    stream->readBoolean();
    stream->readBoolean();
    stream->readBoolean();
    stream->readBoolean();
    int tileArrayStartX, tileArrayStartY, tileArrayEndX, tileArrayEndY;
    if (battleClient->tileMap->mapWidth < 22) {
        tileArrayStartX = stream->readPositiveIntMax31();
        tileArrayStartY = stream->readPositiveIntMax63();
        tileArrayEndX = stream->readPositiveIntMax31();
    }
    else {
        tileArrayStartX = stream->readPositiveIntMax63();
        tileArrayStartY = stream->readPositiveIntMax63();
        tileArrayEndX = stream->readPositiveIntMax63();
    }
    tileArrayEndY = stream->readPositiveIntMax63();

    for (int x = tileArrayStartX;x <= tileArrayEndX;x++) {
        for (int y = tileArrayStartY;y <= tileArrayEndY;y++) {
            auto tile = battleClient->tileMap->getTile(x, y);
            if (tile->shouldEncodeStatus)
                stream->readBoolean();
            if (tile->originalData->blocksProjectiles())
                stream->readBoolean();
        }
    }
    if (stream->readPositiveVIntMax65535OftenZero() != 0) abort();//new tiles
    if (stream->readPositiveVIntMax65535OftenZero() != 0) abort();//idk
    if (stream->readPositiveVIntMax65535OftenZero() != 0) abort();//petrols

    for (auto i : players) {
        if (i->playerIndex == battleClient->ownPlayerIndex) {
            i->ultiCharge = stream->readPositiveIntMax4095();
            auto a = stream->readPositiveIntMax3();
            i->ocCharge = stream->readPositiveIntMax4095();
            auto b = stream->readBoolean();
            auto c = stream->readBoolean();
            auto d = stream->readPositiveVIntMax255OftenZero();
            //todo: modifier 43 special case
            if (stream->readBoolean()) {
                stream->readPositiveIntMax3();
                stream->readPositiveIntMax16383();
            }
        }
        int newHeroIndex = stream->readPositiveIntMax15();
        if (i->heroIndex != newHeroIndex)
            i->setHeroIndex(newHeroIndex);
        if (stream->readBoolean()) abort();//extra data reference
        if (i->accessory != nullptr) {
            i->accessory->decode(stream, i->playerIndex == battleClient->ownPlayerIndex, overwriteCurrentState);
            //todo: modifier 41 special case
            i->accessoryCharges = stream->readPositiveIntMax7();
        }
        stream->readBoolean();
        stream->readBoolean();
        if (stream->readBoolean()) {
            stream->readIntMax15();
            stream->readIntMax15();
        }
        if (stream->readBoolean()) abort();
        stream->readBoolean();
        switch (battleClient->gameModeVariation) {
        case 6:
        case 34:
        case 39:
            stream->readPositiveIntMax15();
        }
        if (battleClient->gameModeVariation == 3)
            stream->readBoolean();//blue star
        auto a = stream->readPositiveVIntMax255OftenZero();
        auto b = stream->readBoolean();
        auto c = stream->readBoolean();//using spray
        if (stream->readBoolean()) {//using pin
            auto d = stream->readIntMax7();
            auto e = stream->readPositiveIntMax16383();
        }
        stream->readBoolean();
    }
    stream->readPositiveVIntMax65535OftenZero();
    stream->readPositiveVIntMax255OftenZero();
    if (battleClient->gmvData->hasTimerAndCanEndBeforeTimerRunsOut())
        stream->readPositiveVIntMax65535();
    if (battleClient->gmvData->roundResetsWhenObjectiveIsMissing()) {
        stream->readBoolean();
        stream->readIntMax1();
    }
    if (LogicGameModeUtil::isDuoMode(battleClient->gameModeVariation)) {
        stream->readPositiveIntMax7();
    }
    else if (battleClient->gameModeVariation == 6 || battleClient->gameModeVariation == 34) {
        stream->readPositiveIntMax15();
    }
    else if (battleClient->gameModeVariation == 16) {
        stream->readPositiveIntMax8191();
        stream->readPositiveIntMax8191();
    }
    else if (battleClient->gmvData->hasTwoBases()) {
        abort();//todo
    }
    else if (battleClient->gmvData->isTypeOfBossFight() || battleClient->gameModeVariation == 7) {
        abort();//todo
    }
    else if (LogicGameModeUtil::isModeWith2Bars(battleClient->gameModeVariation)) {
        abort();//todo
    }
    else if (LogicGameModeUtil::isPayloadSingle(battleClient->gameModeVariation)) {
        abort();//todo
    }
    else switch (battleClient->gameModeVariation) {
    case 13://Training
        int dps = stream->readPositiveVIntMax16777215OftenZero();
        // printf("dps: %d\n", dps);
        break;
    }
    for (auto i : players) {
        if (stream->readBoolean()) abort();
        if (stream->readBoolean()) abort();
    }
    int gameObjectCount = stream->readPositiveVIntMax65535();
    LogicData** gameObjectDatas = new LogicData * [gameObjectCount];
    std::vector<LogicGameObjectClient*> gameObjectsNew;
    for (int i = 0;i < gameObjectCount;i++) gameObjectDatas[i] = stream->readDataReference();
    for (int i = 0;i < gameObjectCount;i++) {
        auto data = gameObjectDatas[i];
        int classID;
        if (data->isCharacter()) classID = 1;
        else if (data->isProjectile()) classID = 2;
        else if (data->isAreaEffect()) classID = 3;
        else classID = 4;
        int globalID = stream->readObjectRunningId(classID);
        for (int j = 0;j < gameObjects.size();j++) {
            if (gameObjects[j]->getGlobalID() == globalID) {
                if (gameObjects[j]->getData() == data) {
                    gameObjectsNew.push_back(gameObjects[j]);
                    gameObjects.erase(gameObjects.begin() + j);
                    goto done;
                }
                else break;
            }
        }
        //new
        LogicGameObjectClient* gameObject;
        if (ownObjectID == globalID) {
            gameObject = new LogicCharacterClientOwn(data);
        }
        else switch (classID) {
        case 1:
            gameObject = new LogicCharacterClient(data);
            break;
        case 2:
            gameObject = new LogicProjectileClient(data);
            break;
        case 3:
            gameObject = new LogicAreaEffectClient(data);
            break;
        default:
            abort();
        }
        gameObject->globalID = globalID;
        gameObjectsNew.push_back(gameObject);
    done:
        ;
    }
    delete[] gameObjectDatas;
    for (auto g : gameObjects) delete g;
    gameObjects = gameObjectsNew;
    if (stream->readBoolean()) abort();//enable movement history
    for (auto g : gameObjects) {
        if (g->getType() != 0) {
            // abort();//todo
        }
        else {
            ((LogicCharacterClient*)g)->decode(stream, g->getGlobalID() == ownObjectID, battleClient, players);
            // break;
        }
    }
    if (Starrpark::isNullsBrawl()) stream->readBoolean();

    assert(stream->isAtEnd());
}

void LogicGameObjectManagerClient::debugLog() {
    printf("====================================\nGame Object Count: %d\n", gameObjects.size());
    for (auto g : gameObjects) {
        printf("%s  globalID: %d  x: %d  y: %d\n", g->getData()->getName().c_str(), g->getGlobalID(), g->getX(), g->getY());
    }
    printf("====================================\n");
}
