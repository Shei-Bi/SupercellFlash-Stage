#include "data/LogicDataTables.h"
// #include "data/LogicDataTable.hpp"
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