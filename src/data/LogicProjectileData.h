#pragma once
#include "LogicData.h"
class CSVTable;
class LogicProjectileData :public LogicData {
public:
    static inline int sm_columnIndexIgnoreLevelBoarder = -1;
    static inline int sm_columnIndexUniqueProperty = -1;

    LogicProjectileData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);

    bool isProjectile();

    int uniqueProperty;
    bool ignoreLevelBoarder;
};