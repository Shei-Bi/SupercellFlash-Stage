#include "data/LogicDataTables.h"
#include "LogicDataTables.h"
#include "GlobalID.h"
#include "data/LogicDataTable.hpp"
#include "LogicGameModeVariationData.h"
#include "LogicLocationData.h"

LogicDataTable* TABLES[101];
void LogicDataTables::initDataTable(CSVNode* csvNode, int index) {
    if (TABLES[index]) {
        ;
    }
    else {
        TABLES[index] = new LogicDataTable(csvNode->table, index);
    }
}
bool LogicDataTables::isLoaded() {
    return sm_loaded;
}
LogicLocationData* LogicDataTables::getLocationByName(const std::string& name) {
    return (LogicLocationData*)TABLES[Location]->getDataByName(name, nullptr);
}
LogicLocationData* LogicDataTables::getTrainingGroundsData() {
    return getLocationByName(std::string("Training"));
}
LogicGameModeVariationData* LogicDataTables::getGameModeVariationByName(const std::string& name) {
    return (LogicGameModeVariationData*)TABLES[GameModeVariation]->getDataByName(name, nullptr);
}
LogicGameModeVariationData* LogicDataTables::getGameModeVariationData(int v) {
    assert(v >= 0 && v < TABLES[GameModeVariation]->getItemCount());
    return (LogicGameModeVariationData*)TABLES[GameModeVariation]->getItemAt(v);
}
LogicAccessoryData* LogicDataTables::getAccessoryByName(const std::string& name) {
    return (LogicAccessoryData*)TABLES[Accessory]->getDataByName(name, nullptr);
}
LogicAreaEffectData* LogicDataTables::getAreaEffectByName(const std::string& name) {
    return (LogicAreaEffectData*)TABLES[AreaEffect]->getDataByName(name, nullptr);
}
LogicSkillData* LogicDataTables::getSkillByName(const std::string& name) {
    return (LogicSkillData*)TABLES[Skill]->getDataByName(name, nullptr);
}
LogicProjectileData* LogicDataTables::getProjectileByName(const std::string& name) {
    return (LogicProjectileData*)TABLES[Projectile]->getDataByName(name, nullptr);
}
LogicCharacterData* LogicDataTables::getCharacterByName(const std::string& name) {
    return (LogicCharacterData*)TABLES[Character]->getDataByName(name, nullptr);
}
LogicSkinConfData* LogicDataTables::getSkinConfByName(const std::string& name) {
    return (LogicSkinConfData*)TABLES[SkinConf]->getDataByName(name, nullptr);
}
LogicSkinData* LogicDataTables::getSkinByName(const std::string& name) {
    return (LogicSkinData*)TABLES[Skin]->getDataByName(name, nullptr);
}
LogicData* LogicDataTables::getDataById(int globalID) {
    if (GlobalID::getClassID(globalID) >= 101) return nullptr;
    return TABLES[GlobalID::getClassID(globalID)]->getItemById(globalID);
}
LogicTileData* LogicDataTables::getOpenTileData() {
    return sm_pOpenTileData;
}
LogicAnimationData* LogicDataTables::getNonConstAnimationByName(const std::string& name) {
    return (LogicAnimationData*)TABLES[Animati0n]->getDataByName(name, nullptr);
}
LogicFaceData* LogicDataTables::getFaceByName(const std::string& name) {
    return (LogicFaceData*)TABLES[Face]->getDataByName(name, nullptr);
}
void LogicDataTables::createReferences() {
    for (int i = 0;i < 101;i++) {
        if (TABLES[i]) TABLES[i]->createReferences();
    }
    sm_pOpenTileData = (LogicTileData*)TABLES[Tile]->getDataByName("Open", nullptr);
    sm_loaded = true;
}