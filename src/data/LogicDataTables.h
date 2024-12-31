#ifndef LOGICDATATABLES_H
#define LOGICDATATABLES_H
#include "data/LogicDataTable.hpp"
extern LogicDataTable* TABLES[61];
enum DataType {
    LocationTheme = 47
};
class LogicDataTables {
public:
    static inline bool sm_loaded = false;
    static void initDataTable(CSVNode* csvNode, int index);
    static bool isLoaded();
};
#endif