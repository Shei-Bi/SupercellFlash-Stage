#include "LogicLocationData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

// int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorRed = -1;
// int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorGreen = -1;
// int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorBlue = -1;
LogicLocationData::LogicLocationData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicLocationData::resetStatics(CSVTable* table) {
    sm_columnIndexVariation = table->getColumnIndexByName("GameModeVariation");
    // sm_columnIndexMapPreviewBGColorRed = table->getColumnIndexByName("MapPreviewBGColorRed");
    // sm_columnIndexMapPreviewBGColorGreen = table->getColumnIndexByName("MapPreviewBGColorGreen");
    // sm_columnIndexMapPreviewBGColorBlue = table->getColumnIndexByName("MapPreviewBGColorBlue");
}
void LogicLocationData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    gameModeData = LogicDataTables::getGameModeVariationByName(csvRow->getValueAt(sm_columnIndexVariation));
}