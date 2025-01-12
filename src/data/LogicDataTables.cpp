#include "data/LogicDataTables.h"
#include "LogicDataTables.h"
#include "GlobalID.h"
#include "data/LogicDataTable.hpp"
#include "LogicGameModeVariationData.h"
#include "LogicLocationData.h"

LogicDataTable* TABLES[61];
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
LogicData* LogicDataTables::getDataById(int globalID) {
    if (GlobalID::getClassID(globalID) >= 61) return nullptr;
    return TABLES[GlobalID::getClassID(globalID)]->getItemById(globalID);
}
LogicTileData* LogicDataTables::getOpenTileData() {
    return sm_pOpenTileData;
}
void LogicDataTables::createReferences() {
    for (int i = 0;i < 61;i++) {
        if (TABLES[i]) TABLES[i]->createReferences();
    }
    sm_pOpenTileData = (LogicTileData*)TABLES[Tile]->getDataByName("Open", nullptr);
    sm_loaded = true;
}