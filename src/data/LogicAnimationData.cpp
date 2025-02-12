#include "LogicAnimationData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicAnimationData::LogicAnimationData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}

void LogicAnimationData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
}

void LogicAnimationData::resetStatics(CSVTable* table) {
    sm_columnIndexFileName = table->getColumnIndexByName("FileName");
    sm_columnIndexStartFrame = table->getColumnIndexByName("StartFrame");
    sm_columnIndexEndFrame = table->getColumnIndexByName("EndFrame");
    sm_columnIndexLooping = table->getColumnIndexByName("Looping");
    sm_columnIndexTransitionOutMs = table->getColumnIndexByName("TransitionOutMs");
}

const std::string LogicAnimationData::getFileName() {
    return csvRow->getValueAt(sm_columnIndexFileName);
}

int LogicAnimationData::getStartFrame() {
    return csvRow->getIntegerValueAt(sm_columnIndexStartFrame);
}

int LogicAnimationData::getEndFrame() {
    return csvRow->getIntegerValueAt(sm_columnIndexEndFrame);
}

bool LogicAnimationData::getLooping() {
    return csvRow->getBooleanValueAt(sm_columnIndexLooping);
}

int LogicAnimationData::getTransitionOutMs() {
    return csvRow->getIntegerValueAt(sm_columnIndexTransitionOutMs);
}
