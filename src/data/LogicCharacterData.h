#pragma once
#include "LogicData.h"
class CSVTable;

class LogicCharacterData :public LogicData {
public:
    LogicCharacterData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
        ;
    }
};