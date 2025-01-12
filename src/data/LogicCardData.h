#pragma once
#include "LogicData.h"
class CSVTable;

class LogicCardData :public LogicData {
public:
    LogicCardData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
        ;
    }
};