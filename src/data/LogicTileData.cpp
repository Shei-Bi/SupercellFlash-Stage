#include "LogicTileData.h"
#include "csv/CSVTable.h"

LogicTileData::LogicTileData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}

void LogicTileData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}

void LogicTileData::resetStatics(CSVTable* table) {
    sm_columnIndexTileCode = table->getColumnIndexByName("TileCode");
    sm_columnIndexIsDestructible = table->getColumnIndexByName("IsDestructible");
    sm_columnIndexIsDestructibleNormalWeapon = table->getColumnIndexByName("IsDestructibleNormalWeapon");
    sm_columnIndexBlocksMovement = table->getColumnIndexByName("BlocksMovement");
    sm_columnIndexBlocksProjectiles = table->getColumnIndexByName("BlocksProjectiles");
    sm_columnIndexIsDestructibleOvertime = table->getColumnIndexByName("IsDestructibleOvertime");
}

char LogicTileData::getTileCode() {
    return csvRow->getValueAt(sm_columnIndexTileCode)[0];
}

bool LogicTileData::isDestructible() {
    return csvRow->getBooleanValueAt(sm_columnIndexIsDestructible);
}

bool LogicTileData::isDestructibleOvertime() {
    return csvRow->getBooleanValueAt(sm_columnIndexIsDestructibleOvertime);
}

bool LogicTileData::blocksProjectiles() {
    return csvRow->getBooleanValueAt(sm_columnIndexBlocksProjectiles);
}
