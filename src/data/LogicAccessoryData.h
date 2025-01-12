#pragma once
#include "LogicData.h"
class CSVTable;

class LogicAccessoryData :public LogicData {
public:
    LogicAccessoryData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
        ;
    }
};