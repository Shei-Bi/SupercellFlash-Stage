#pragma once
#include "LogicData.h"
#include "LogicAccessoryData.h"

class CSVTable;

class LogicCardData :public LogicData {
    LogicAccessoryData* accessory;
public:
    static inline int sm_columnIndexSkill = -1;
    static inline int sm_columnIndexType = -1;
    LogicCardData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    LogicAccessoryData* getAccessory();
};