#include "LogicAreaEffectData.h"

LogicAreaEffectData::LogicAreaEffectData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {

}

void LogicAreaEffectData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}

void LogicAreaEffectData::resetStatics(CSVTable*) {
    ;
}

bool LogicAreaEffectData::isAreaEffect() {
    return true;
}
