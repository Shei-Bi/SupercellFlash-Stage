#pragma once
#include "LogicData.h"
class CSVTable;

class LogicGearBoostData :public LogicData {
public:
    LogicGearBoostData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
        ;
    }
};