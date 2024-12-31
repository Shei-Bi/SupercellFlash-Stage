#pragma once
#include "LogicData.h"
#include "data/LogicGameModeVariationData.h"
class CSVTable;

class LogicLocationData :public LogicData {
public:
    static inline int sm_columnIndexVariation = -1;
    // static int sm_columnIndexMapPreviewBGColorGreen;
    // static int sm_columnIndexMapPreviewBGColorBlue;
    LogicGameModeVariationData* gameModeData;
    LogicLocationData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    int getGameModeVariation() {
        return gameModeData->getVariation();
    }
};