#pragma once
#include "LogicData.h"
class CSVTable;

class LogicAreaEffectData :public LogicData {
public:
    LogicAreaEffectData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);

    bool isAreaEffect();
};