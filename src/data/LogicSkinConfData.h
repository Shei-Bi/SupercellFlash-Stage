#pragma once
#include "LogicData.h"
class CSVTable;

class LogicSkinConfData :public LogicData {
    static inline int sm_columnIndexModel = -1;
public:
    LogicSkinConfData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);
    std::string getModel();
};