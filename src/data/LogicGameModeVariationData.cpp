#include "LogicGameModeVariationData.h"
#include "csv/CSVTable.h"
LogicGameModeVariationData::LogicGameModeVariationData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicGameModeVariationData::resetStatics(CSVTable* table) {
    sm_columnIndexVariation = table->getColumnIndexByName("Variation");
    sm_columnIndexGameModeIconName = table->getColumnIndexByName("GameModeIconName");
    // sm_columnIndexMapPreviewBGColorGreen = table->getColumnIndexByName("MapPreviewBGColorGreen");
    // sm_columnIndexMapPreviewBGColorBlue = table->getColumnIndexByName("MapPreviewBGColorBlue");
}
void LogicGameModeVariationData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    assert(getInstanceID() == csvRow->getIntegerValueAt(sm_columnIndexVariation));
}