#ifndef LOGICDATATABLES_H
#define LOGICDATATABLES_H
#include "data/LogicDataTable.hpp"
extern LogicDataTable* TABLES[61];
#include "data/LogicGameModeVariationData.h"
enum DataType {
    Location = 15,
    LocationTheme = 47,
    GameModeVariation = 48
};
class LogicDataTables {
public:
    static inline bool sm_loaded = false;
    static void initDataTable(CSVNode* csvNode, int index);
    static void createReferences();
    static bool isLoaded();
    static LogicGameModeVariationData* getGameModeVariationByName(const std::string& name) {
        return (LogicGameModeVariationData*)TABLES[GameModeVariation]->getDataByName(name, nullptr);
    }
    static LogicGameModeVariationData* getGameModeVariationData(int v) {
        assert(v >= 0 && v < TABLES[GameModeVariation]->datas.size());
        return (LogicGameModeVariationData*)TABLES[GameModeVariation]->datas[v];
    }
};
#endif