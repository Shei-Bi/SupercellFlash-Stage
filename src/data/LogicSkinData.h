#pragma once
#include "LogicData.h"
class CSVTable;

class LogicSkinData :public LogicData {
public:
    LogicSkinData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
        ;
    }
};