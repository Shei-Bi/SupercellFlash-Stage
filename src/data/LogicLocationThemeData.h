#pragma once
#include "LogicData.h"
class CSVTable;

class LogicLocationThemeData :public LogicData {
public:
    static int sm_columnIndexMapPreviewBGColorRed;
    static int sm_columnIndexMapPreviewBGColorGreen;
    static int sm_columnIndexMapPreviewBGColorBlue;
    LogicLocationThemeData(CSVRow*, LogicDataTable*);
    // void createReferences();
        // LogicLocationThemeData::resetStatics(TABLES[LocationTheme]->csvTable);

    static void resetStatics(CSVTable*);
};