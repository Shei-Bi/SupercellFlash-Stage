#include "LogicGameObjectManagerClient.h"
#include "LogicBattleModeClient.h"
#include "util/LogicGameModeUtil.hpp"

void LogicGameObjectManagerClient::decode(BitStream* stream, std::vector<LogicPlayer*>& players, bool overwriteCurrentState, LogicBattleModeClient* battleClient, std::vector<LogicPlayer*>* interpolatedProjectiles) {
    ownObjectID = stream->readPositiveIntMax2097151();
    willowObjectID = stream->readPositiveIntMax2097151();
    if (stream->readBoolean()) {//modifiers array
        for (int i = stream->readPositiveIntMax63();i > 0;i--) stream->readPositiveIntMax63();
    }
    if (stream->readBoolean()) stream->readPositiveIntMax255();
    if (LogicGameModeUtil::isGemGrab(battleClient->gameModeVariation)) gemGrabCountDown = stream->readPositiveVIntMax65535();
    else if (LogicGameModeUtil::isKingOfHill(battleClient->gameModeVariation))
        ;
    stream->readBoolean();
    roundState = stream->readIntMax15();
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
            //arcade stuff 43
            if (stream->readBoolean()) {
                stream->readPositiveIntMax3();
                stream->readPositiveIntMax16383();
            }
            i->heroIndex = stream->readPositiveIntMax15();
            if (stream->readBoolean()) abort();//extra data reference
        }
    }
}
