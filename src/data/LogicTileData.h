#pragma once
#include "LogicData.h"
class CSVTable;

class LogicTileData :public LogicData {
    static inline int sm_columnIndexTileCode = -1;
    static inline int sm_columnIndexIsDestructible = -1;
    static inline int sm_columnIndexIsDestructibleNormalWeapon = -1;
    static inline int sm_columnIndexBlocksMovement = -1;
    static inline int sm_columnIndexBlocksProjectiles = -1;
    static inline int sm_columnIndexIsDestructibleOvertime = -1;
public:
    LogicTileData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);
    char getTileCode();
    bool isDestructible();
    bool isDestructibleOvertime();
    bool blocksProjectiles();
};