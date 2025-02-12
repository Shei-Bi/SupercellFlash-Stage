#pragma once
#include "LogicData.h"
class CSVTable;

class LogicCharacterData;
class LogicSkinConfData :public LogicData {
    static inline int sm_columnIndexModel = -1;
    static inline int sm_columnIndexIdleAnim = -1;
    static inline int sm_columnIndexIdleFace = -1;
    static inline int sm_columnIndexHappyAnim = -1;
    static inline int sm_columnIndexHappyFace = -1;
    static inline int sm_columnIndexHappyLoopAnim = -1;
    static inline int sm_columnIndexHappyLoopFace = -1;
    static inline int sm_columnIndexLobbyAnim = -1;
    static inline int sm_columnIndexLobbyFace = -1;
    static inline int sm_columnIndexLobbyLoopAnim = -1;
    static inline int sm_columnIndexLobbyLoopFace = -1;
    static inline int sm_columnIndexCharacter = -1;
public:
    LogicCharacterData* character;
    LogicSkinConfData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);
    std::string getModel();
    const std::string getIdleAnim();
    const std::string getIdleFace();
    const std::string getHappyAnim();
    const std::string getHappyFace();
    const std::string getHappyLoopAnim();
    const std::string getHappyLoopFace();
    const std::string getLobbyAnim();
    const std::string getLobbyFace();
    const std::string getLobbyLoopAnim();
    const std::string getLobbyLoopFace();
};