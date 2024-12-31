#pragma once
#include "LogicData.h"
class CSVTable;

class LogicGameModeVariationData :public LogicData {
public:
    static inline int sm_columnIndexVariation = -1;
    static inline int sm_columnIndexGameModeIconName = -1;
    // static int sm_columnIndexMapPreviewBGColorGreen;
    // static int sm_columnIndexMapPreviewBGColorBlue;
    LogicGameModeVariationData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    int getVariation() {
        return csvRow->getIntegerValueAt(sm_columnIndexVariation);
    }
    const std::string& getGameModeIconName() {
        return csvRow->getValueAt(sm_columnIndexGameModeIconName);
    }
};