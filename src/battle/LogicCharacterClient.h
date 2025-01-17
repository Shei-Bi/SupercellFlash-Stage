#pragma once
#include "LogicGameObjectClient.h"
#include "LogicBattleModeClient.h"
#include "LogicGear.h"
#include "data/LogicCharacterData.h"
#include "LogicSkillClient.h"
#include "util/LogicGameModeUtil.hpp"

class LogicCharacterClient :public LogicGameObjectClient {
public:
    bool chargeUpCalculated;
    std::vector<LogicGear*> gears;
    int state;
    int moveAngle;
    int attackAngle;
    bool isPlayerControlRemoved;
    bool isForcedAngle;
    bool hasDamageBoost;
    int attackAnimation;
    int hitpoints;
    int hitpointsMax;
    int consumableShield;
    int consumableShieldMax;
    bool isCharging;
    std::vector<LogicSkillClient*> skills;

    LogicCharacterClient(LogicData* data) : LogicGameObjectClient(data) {
        chargeUpCalculated = false;
        isCharging = false;
        if (((LogicCharacterData*)data)->weaponSkill) {
            skills.push_back(new LogicSkillClient(((LogicCharacterData*)data)->weaponSkill));
        }
        if (((LogicCharacterData*)data)->ultimateSkill) {
            skills.push_back(new LogicSkillClient(((LogicCharacterData*)data)->ultimateSkill));
        }
    }
    ~LogicCharacterClient() {
        for (auto g : gears) delete g;
        for (auto s : skills) delete s;
    }
    void decode(BitStream* stream, bool isOwn, LogicBattleModeClient* battleClient, std::vector<LogicPlayer*>& players) {
        LogicGameObjectClient::decode(stream, battleClient->gameModeVariation, battleClient->tileMap->mapWidth > 21);

        LogicPlayer* player = playerIndex >= 0 ? players[playerIndex] : nullptr;

        if (player)
            player->objectGlobalID = globalID;

        calculateChargeUp(player);

        auto data = (LogicCharacterData*)getData();
        if (isObject()) {
            state = stream->readPositiveIntMax7();
            if (data->isTrainOrPayload() || data->hasManualRotations) {
                attackAngle = stream->readPositiveIntMax511();
                moveAngle = stream->readPositiveIntMax511();
            }
            else if (data->areaEffect != nullptr) {
                attackAngle = stream->readPositiveIntMax511();
            }
        }
        else {
            bool flag = false;
            auto idk1 = stream->readBoolean();
            if (idk1) stream->readIntMax15();
            if (isOwn) {
                isPlayerControlRemoved = stream->readBoolean();
                isForcedAngle = stream->readBoolean();
                auto idk = stream->readBoolean();
                if (idk1 || isPlayerControlRemoved || isForcedAngle || idk) {
                    attackAngle = stream->readPositiveIntMax511();
                    moveAngle = stream->readPositiveIntMax511();
                    flag = true;
                }
            }
            else {
                attackAngle = stream->readPositiveIntMax511();
                moveAngle = stream->readPositiveIntMax511();
            }
            state = stream->readPositiveIntMax7();
            if (state == 0 && !flag) moveAngle = attackAngle;
            hasDamageBoost = stream->readBoolean();
            attackAnimation = stream->readIntMax63();
            auto reversedAngle = stream->readBoolean();
            if (stream->readBoolean()) stream->readBoolean();
            auto a = stream->readBoolean();
            auto b = stream->readBoolean();
        }
        if (data->getLifeTimeTicks() >= 1) {
            auto ltt = stream->readPositiveIntMax511();
        }
        stream->readPositiveVIntMax65535OftenZero();
        stream->readPositiveVIntMax65535OftenZero();
        bool c = stream->readBoolean();
        bool d = stream->readBoolean();
        bool speedFaster = stream->readBoolean();
        bool speedSlower = stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        if (stream->readBoolean()) {
            stream->readPositiveIntMax7();
        }
        stream->readBoolean();
        stream->readBoolean();
        if (stream->readBoolean()) {
            stream->readIntMax15();
        }
        if (stream->readBoolean()) {
            stream->readIntMax15();
        }
        if (stream->readBoolean()) {
            stream->readIntMax127();
        }
        auto f = stream->readPositiveVIntMax255OftenZero();
        if (stream->readBoolean()) abort();
        stream->readBoolean();
        stream->readPositiveVIntMax255OftenZero();
        hitpoints = stream->readPositiveVIntMax16777215();
        hitpointsMax = stream->readPositiveVIntMax16777215();
        if (hitpoints < 1) stream->readIntMax15();
        if (data->isDecoy()) {
            stream->readBoolean();
        }
        if (stream->readBoolean()) {
            stream->readPositiveIntMax511();
        }
        if (data->isHero() || data->isDecoy()) {
            if (battleClient->gmvData->isPlayerCollectBountyStars()) {
                int starCount = stream->readPositiveVIntMax255OftenZero() + 2;
            }
            else {
                int starCount = stream->readPositiveVIntMax255OftenZero();
                if (starCount > 0 && LogicGameModeUtil::isCollectItemsWithTimer(battleClient->gameModeVariation)) {
                    int timer = stream->readPositiveVIntMax255();
                }
            }
            if (battleClient->gameModeVariation == 35) stream->readPositiveVIntMax65535OftenZero();
            else stream->readPositiveVIntMax255OftenZero();
            stream->readBoolean();
            if (hitpoints <= 0) stream->readBoolean();
        }
        if (data->isHero() || data->canGainShield()) {
            if (stream->readBoolean()) {
                consumableShield = stream->readPositiveIntMax16383();
                consumableShieldMax = stream->readPositiveIntMax16383();
            }
            else {
                consumableShield = 0;
                consumableShieldMax = 0;
            }
        }
        if (data->getUniqueProperty() == 24 || data->isMinionTwin())
            stream->readBoolean();
        if (data->isHero()) {
            if (battleClient->gmvData->modeHasCarryables()) abort();//todo
            stream->readPositiveVIntMax255OftenZero();
            int size = battleClient->gameModeVariation == 35 ? stream->readIntMax7() : (int)stream->readBoolean();
            if (stream->readBoolean()) {
                isCharging = stream->readBoolean();
                stream->readBoolean();
                stream->readBoolean();
                stream->readBoolean();
                stream->readBoolean();
            }
            else {
                isCharging = false;
            }
            if (player->hasOverchargeData()) {
                bool ocReady = stream->readBoolean();
                bool ocing = stream->readBoolean();
                bool ocOnStart = stream->readBoolean();
                bool ocOnEnd = stream->readBoolean();
            }
            if (isOwn) {
                if (stream->readBoolean()) {//visionOverride
                    stream->readIntMax65535();
                    stream->readIntMax65535();
                }
            }
            if (data->weaponSkill) {
                if (data->weaponSkill->getChargedShotCount() >= 1)
                    auto chargedShootCount = stream->readIntMax3();
                if (isOwn && data->weaponSkill->getAttackPattern() == 13)
                    auto chargeUp = stream->readPositiveVIntMax255OftenZero();
                if (data->weaponSkill->getAttackPattern() == 15 || data->weaponSkill->getAttackPattern() == 17) {
                    if (stream->readPositiveVIntMax255OftenZero() >= 1) {
                        stream->readPositiveIntMax511();
                    }
                }
            }
            if (data->ultimateSkill && data->ultimateSkill->getChargeType() == 15) stream->readBoolean();
            if (data->shouldEncodePetStatus || data->getUniqueProperty() == 13 && data->isHero()) {
                stream->readBoolean();
            }
            if (data->powerLevelsType == 1) {
                int pl = stream->readPositiveIntMax7();
            }
            else if (data->getUniqueProperty() == 1) {
                int pl = stream->readPositiveIntMax3();
            }
            else if (data->powerLevelsType == 2) {
                abort();
            }
            if (stream->readBoolean()) {
                int a = stream->readPositiveIntMax15();
                int b = stream->readPositiveIntMax7();
            }
            if (isCharging) {
                int chargeAnimation = 50 * stream->readPositiveIntMax255();
                int chargeType = stream->readPositiveIntMax31();
                int c = stream->readPositiveIntMax31();
                if (chargeType == 17) stream->readBoolean();
            }
            else {
                int chargeAnimation = 0;
            }
            if (data->ultimateSkill && data->ultimateSkill->projectile && data->ultimateSkill->projectile->uniqueProperty == 5) {
                if (stream->readBoolean()) {
                    int hookX = stream->readPositiveVIntMax65535();
                    int hookY = stream->readPositiveVIntMax65535();
                }
            }
            if (data->getUniqueProperty() == 19)
                if (stream->readBoolean())
                    stream->readPositiveVIntMax65535();

            //todo: chargeup

            for (auto gear : gears) {
                gear->decode(stream, isOwn);
            }
        }
        else if (data->isDog())
            stream->readPositiveIntMax31();
        else if (data->secondaryPet || data->isMinionTwin()) {
            stream->readPositiveIntMax7();
            stream->readBoolean();
        }
        else if (data->type == Minion_FindEnemies || data->type == Minion_Building) {
            stream->readBoolean();
            stream->readBoolean();
        }
        else if (data->isCarryable()) {
            stream->readPositiveIntMax7();
            stream->readPositiveIntMax3();
        }
        else if ((data->weaponSkill && data->weaponSkill->behaviorType == Charge) || (data->ultimateSkill && data->ultimateSkill->behaviorType == Charge)) {
            isCharging = stream->readBoolean();
            if (isCharging) {
                int chargeType = stream->readPositiveIntMax31();
            }
        }
        else if (data->type == Pvp_Base && battleClient->gameModeVariation == 11) {
            stream->readPositiveIntMax127();
        }

        if (data->petAutoSpawnDelay >= 1) {
            stream->readPositiveIntMax1023();
        }
        if (data->getSpawnedPet()) {
            stream->readBoolean();
        }
        if (data->getUniqueProperty() == 9) {
            stream->readBoolean();
        }
        else if (data->getUniqueProperty() == 28) {
            stream->readBoolean();
            stream->readBoolean();
        }
        else if (data->getUniqueProperty() == 26) {
            stream->readBoolean();
            if (stream->readBoolean()) {
                stream->readIntMax65535();
                stream->readIntMax65535();
            }
        }
        else if (data->getUniqueProperty() == 18) {
            stream->readBoolean();
        }
        else if (data->getUniqueProperty() == 20) {
            if (stream->readBoolean()) {
                stream->readPositiveVIntMax65535();
            }
        }
        if (canUseFastTravel()) {
            stream->readBoolean();
            stream->readBoolean();
        }
        if (data->getUniqueProperty() == 22) {
            if (stream->readBoolean()) {
                stream->readPositiveVIntMax65535();
            }
        }
        if (stream->readBoolean()) {
            stream->readPositiveVIntMax65535();
        }
        if (stream->readBoolean()) {
            stream->readBoolean();
            stream->readIntMax15();
        }
        if (data->isMinionLastStand()) {
            stream->readBoolean();
        }
        if (data->getUniqueProperty() == 11 && isOwn) {
            stream->readIntMax255();
            stream->readPositiveIntMax7();
        }
        if (battleClient->gameModeVariation == 14 && data->isBoss()) {
            stream->readBoolean();
        }
        //bosswithdifferentstages
        if (data->getUniqueProperty() == 32) {
            if (stream->readBoolean())
                stream->readPositiveIntMax511();
        }
        auto hasShield = stream->readBoolean();
        auto visibilityState = stream->readPositiveIntMax3();
        auto invisible = stream->readBoolean();
        stream->readPositiveIntMax511();
        stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        stream->readBoolean();
        if (worldIndex == 1) {
            stream->readPositiveIntMax255();
            stream->readPositiveIntMax255();
            stream->readIntMax15();
        }
        if (isOwn) {
            if (stream->readBoolean()) {
                auto speedIncrease = stream->readIntMax1023();//speed increase
            }
            stream->readBoolean();
        }
        for (int i = stream->readPositiveIntMax31();i > 0;i--) {
            stream->readIntMax32767();
            stream->readBoolean();
        }
        for (auto skill : skills) {
            skill->decode(stream, isOwn, this, battleClient);
        }
        stream->readPositiveIntMax3();
        stream->readBoolean();
        if (stream->readBoolean()) abort();
        if (stream->readBoolean()) stream->readBoolean();
        if (stream->readBoolean()) {
            if (data->isHero()) {
                stream->readBoolean();
                stream->readBoolean();
            }
        }
        stream->readPositiveVIntMax16777215();
        if (battleClient->gameModeVariation == 48) {
            if (data->isHero()) {
                stream->readPositiveIntMax131071();
            }
        }
    }
    int getType() {
        return 0;
    }
    void calculateChargeUp(LogicPlayer* player) {
        if (chargeUpCalculated) return;
        createGears(player);
        chargeUpCalculated = true;
    }
    void createGears(LogicPlayer* player) {
        if (player == nullptr) return;
        if (!((LogicCharacterData*)getData())->isHero()) return;
        auto upgrades = player->heroes[player->heroIndex]->upgrades;
        if (upgrades == nullptr) return;
        if (upgrades->gear1 != nullptr) gears.push_back(new LogicGear(upgrades->gear1));
        if (upgrades->gear2 != nullptr) gears.push_back(new LogicGear(upgrades->gear2));
    }
    bool isObject() {
        auto data = (LogicCharacterData*)getData();
        return !data->hasAutoAttack() && data->getSpeed() == 0 && data->type != Minion_Building_charges_ulti && data->getUniqueProperty() != 5;
    }
    bool canUseFastTravel() {
        auto data = ((LogicCharacterData*)getData());
        return  (data->isHero() || isPet()) && data->getUniqueProperty() != 26;
    }
    bool isPet() {
        return playerIndex >= 0 && !((LogicCharacterData*)getData())->isHero();
    }
};