#include "LogicLocationData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicLocationData::LogicLocationData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicLocationData::resetStatics(CSVTable* table) {
    sm_columnIndexVariation = table->getColumnIndexByName("GameModeVariation");
    sm_columnIndexMap = table->getColumnIndexByName("Map");
}
void LogicLocationData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    gameModeData = LogicDataTables::getGameModeVariationByName(csvRow->getValueAt(sm_columnIndexVariation));
}