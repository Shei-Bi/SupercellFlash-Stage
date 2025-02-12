#include "LogicFaceData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicFaceData::LogicFaceData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}

void LogicFaceData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}

void LogicFaceData::resetStatics(CSVTable* table) {
    sm_columnIndexFileName = table->getColumnIndexByName("FileName");
    sm_columnIndexExportName = table->getColumnIndexByName("ExportName");
}

const std::string LogicFaceData::getFileName() {
    return csvRow->getValueAt(sm_columnIndexFileName);
}

const std::string LogicFaceData::getExportName() {
    return csvRow->getValueAt(sm_columnIndexExportName);
}
