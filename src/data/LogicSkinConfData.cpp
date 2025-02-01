#include "LogicSkinConfData.h"
#include "csv/CSVTable.h"

LogicSkinConfData::LogicSkinConfData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}

void LogicSkinConfData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}

void LogicSkinConfData::resetStatics(CSVTable* table) {
    sm_columnIndexModel = table->getColumnIndexByName("Model");
}

std::string LogicSkinConfData::getModel() {
    return csvRow->getValueAt(sm_columnIndexModel);
}
