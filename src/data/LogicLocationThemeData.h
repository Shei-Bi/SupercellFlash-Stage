#pragma once
#include "LogicData.h"
class CSVTable;

class LogicLocationThemeData :public LogicData {
public:
    static int sm_columnIndexMapPreviewBGColorRed;
    static int sm_columnIndexMapPreviewBGColorGreen;
    static int sm_columnIndexMapPreviewBGColorBlue;
    LogicLocationThemeData(CSVRow*, LogicDataTable*);
    static void resetStatics(CSVTable*);
};