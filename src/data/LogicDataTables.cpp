#include "data/LogicDataTables.h"
#include "LogicDataTables.h"
#include "GlobalID.h"

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
LogicData* LogicDataTables::getDataById(int globalID) {
    if (GlobalID::getClassID(globalID) >= 61) return nullptr;
    return TABLES[GlobalID::getClassID(globalID)]->getItemById(globalID);
}
void LogicDataTables::createReferences() {
    for (int i = 0;i < 61;i++) {
        if (TABLES[i]) TABLES[i]->createReferences();
    }
    sm_loaded = true;
}