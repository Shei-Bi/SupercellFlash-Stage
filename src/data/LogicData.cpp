#include "LogicData.h"
#include <stdlib.h>
#include "GlobalID.h"
#include "data/LogicDataTable.hpp"

LogicData::LogicData() {
    abort();
}
LogicData::LogicData(CSVRow* csvRow, LogicDataTable* parentTable) :csvRow(csvRow), parentTable(parentTable) {
    globalID = GlobalID::createGlobalID(parentTable->getTableIndex(), parentTable->getItemCount());
}
void LogicData::setCSVRow(CSVRow* csvRow, int size) {
    this->csvRow = csvRow;
    rowSize = size;
    iconSWF = csvRow->getValue("IconSWF", 0);
    iconExportName = csvRow->getValue("IconExportName", 0);
}
const std::string& LogicData::getName() {
    return csvRow->getName();
}
const std::string& LogicData::getIconSWF() {
    return iconSWF;
}
const std::string& LogicData::getIconExportName() {
    return iconExportName;
}
void LogicData::createReferences() {
    ;
}
int LogicData::getInstanceID() {
    return GlobalID::getInstanceID(globalID);
}

bool LogicData::isCharacter() {
    return false;
}
bool LogicData::isProjectile() {
    return false;
}
bool LogicData::isAreaEffect() {
    return false;
}
bool LogicData::isItem() {
    return false;
}
