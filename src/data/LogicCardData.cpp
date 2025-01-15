#include "LogicCardData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicCardData::LogicCardData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    accessory = nullptr;
}

void LogicCardData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    auto const& type = csvRow->getValueAt(sm_columnIndexType);
    if (type == "accessory") accessory = LogicDataTables::getAccessoryByName(csvRow->getValueAt(sm_columnIndexSkill));
}

void LogicCardData::resetStatics(CSVTable* table) {
    sm_columnIndexSkill = table->getColumnIndexByName("Skill");
    sm_columnIndexType = table->getColumnIndexByName("Type");
}

LogicAccessoryData* LogicCardData::getAccessory() {
    return accessory;
}
