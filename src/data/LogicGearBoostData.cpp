#include "LogicGearBoostData.h"
#include "csv/CSVTable.h"

LogicGearBoostData::LogicGearBoostData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicGearBoostData::resetStatics(CSVTable* table) {
    sm_columnIndexLogicType = table->getColumnIndexByName("LogicType");
}
int LogicGearBoostData::getLogicType() {
    return csvRow->getIntegerValueAt(sm_columnIndexLogicType);
}
void LogicGearBoostData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}