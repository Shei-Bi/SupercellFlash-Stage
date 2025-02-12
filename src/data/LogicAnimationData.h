#pragma once
#include "LogicData.h"
#include "LogicAccessoryData.h"

class CSVTable;

class LogicAnimationData :public LogicData {
public:
    static inline int sm_columnIndexFileName = -1;
    static inline int sm_columnIndexStartFrame = -1;
    static inline int sm_columnIndexEndFrame = -1;
    static inline int sm_columnIndexLooping = -1;
    static inline int sm_columnIndexTransitionOutMs = -1;
    LogicAnimationData(CSVRow*, LogicDataTable*);
    void createReferences();
    static void resetStatics(CSVTable*);
    const std::string getFileName();
    int getStartFrame();
    int getEndFrame();
    bool getLooping();
    int getTransitionOutMs();
};