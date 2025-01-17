#pragma once
#include "LogicData.h"
class CSVTable;

class LogicGameModeVariationData :public LogicData {
public:
    static inline int sm_columnIndexVariation = -1;
    static inline int sm_columnIndexGameModeIconName = -1;
    static inline int sm_columnIndexHasTimerAndCanEndBeforeTimerRunsOut = -1;
    static inline int sm_columnIndexRoundResetsWhenObjectiveIsMissing = -1;
    static inline int sm_columnIndexHasTwoBases = -1;
    static inline int sm_columnIndexIsTypeOfBossFight = -1;
    static inline int sm_columnIndexTeamSize = -1;
    static inline int sm_columnIndexTeamCount = -1;
    static inline int sm_columnIndexPlayerCollectBountyStars = -1;
    static inline int sm_columnIndexModeHasCarryables = -1;

    LogicGameModeVariationData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    int getVariation();
    const std::string& getGameModeIconName();
    bool hasTimerAndCanEndBeforeTimerRunsOut();
    bool roundResetsWhenObjectiveIsMissing();
    bool hasTwoBases();
    bool isTypeOfBossFight();
    int getPlayerCount();
    bool isPlayerCollectBountyStars();
    bool modeHasCarryables();
};