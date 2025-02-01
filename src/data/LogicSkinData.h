#pragma once
#include "LogicData.h"
#include "LogicSkinConfData.h"

class CSVTable;

class LogicSkinData :public LogicData {
    static inline int sm_columnIndexConf = -1;
    static inline int sm_columnIndexDiffuseTexture = -1;

public:
    LogicSkinConfData* conf;
    std::string diffuseTexture;
    LogicSkinData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);
};