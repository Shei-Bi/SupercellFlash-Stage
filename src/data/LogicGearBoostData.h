#pragma once
#include "LogicData.h"
class CSVTable;

class LogicGearBoostData :public LogicData {
public:
    static inline int sm_columnIndexLogicType = -1;

    LogicGearBoostData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);

    int getLogicType();
};