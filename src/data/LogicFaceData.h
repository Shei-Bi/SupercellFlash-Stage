#pragma once
#include "LogicData.h"
#include "LogicAccessoryData.h"

class CSVTable;

class LogicFaceData :public LogicData {
public:
    static inline int sm_columnIndexFileName = -1;
    static inline int sm_columnIndexExportName = -1;
    LogicFaceData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    const std::string getFileName();
    const std::string getExportName();
};