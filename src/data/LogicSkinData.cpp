#include "LogicSkinData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicSkinData::LogicSkinData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}

void LogicSkinData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    conf = LogicDataTables::getSkinConfByName(csvRow->getValueAt(sm_columnIndexConf));
    diffuseTexture = csvRow->getValueAt(sm_columnIndexDiffuseTexture);
}

void LogicSkinData::resetStatics(CSVTable* table) {
    sm_columnIndexConf = table->getColumnIndexByName("Conf");
    sm_columnIndexDiffuseTexture = table->getColumnIndexByName("DiffuseTexture");
}