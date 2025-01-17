#include "LogicProjectileData.h"
#include "csv/CSVTable.h"

LogicProjectileData::LogicProjectileData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicProjectileData::resetStatics(CSVTable* table) {
    sm_columnIndexIgnoreLevelBoarder = table->getColumnIndexByName("IgnoreLevelBoarder");
    sm_columnIndexUniqueProperty = table->getColumnIndexByName("UniqueProperty");
}
void LogicProjectileData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    ignoreLevelBoarder = csvRow->getBooleanValueAt(sm_columnIndexIgnoreLevelBoarder);
    uniqueProperty = csvRow->getIntegerValueAt(sm_columnIndexUniqueProperty);
}
bool LogicProjectileData::isProjectile() {
    return true;
}