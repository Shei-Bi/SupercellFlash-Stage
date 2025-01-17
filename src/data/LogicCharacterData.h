#pragma once
#include "LogicData.h"
#include "LogicAreaEffectData.h"
#include "LogicSkillData.h"

class CSVTable;
enum CharacterType {
    Hero,
    Npc_Boss,
    Minion_FollowOwner,
    Minion_FindEnemies,
    Minion_Building,
    Campaign_DefendTarget,
    Pvp_Base,
    Minion_Building_charges_ulti,
    Minion_Dog,
    Unused,
    LootBox,
    Minion_FindEnemies2,
    RoboWars,
    Train,
    Unused2,
    Minion_Mirage,
    Npc_Boss_TownCrush,
    Carryable,
    Minion_Duplicate,
    Payload,
    Minion_Invasion,
    Minion_LastStand,
    Minion_Percenter,
    Minion_Twin,
    Minion_Critter,
    Minion_Orbiting,
    Her0,
    M
};
class LogicCharacterData :public LogicData {
public:

    static inline int sm_columnIndexType = -1;
    static inline int sm_columnIndexDamage = -1;
    static inline int sm_columnIndexSpeed = -1;
    static inline int sm_columnIndexUniqueProperty = -1;
    static inline int sm_columnIndexUniquePropertyValue1 = -1;
    static inline int sm_columnIndexUniquePropertyValue2 = -1;
    static inline int sm_columnIndexManualRotations = -1;
    static inline int sm_columnIndexAreaEffect = -1;
    static inline int sm_columnIndexLifeTimeTicks = -1;
    static inline int sm_columnIndexSecondaryPet = -1;
    static inline int sm_columnIndexIsExternalPet = -1;
    static inline int sm_columnIndexWeaponSkill = -1;
    static inline int sm_columnIndexUltimateSkill = -1;
    static inline int sm_columnIndexShouldEncodePetStatus = -1;
    static inline int sm_columnIndexPowerLevelsType = -1;
    static inline int sm_columnIndexCarryableType = -1;
    static inline int sm_columnIndexPetAutoSpawnDelay = -1;
    static inline int sm_columnIndexPet = -1;

    LogicCharacterData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);

    int type;
    bool hasManualRotations;
    LogicAreaEffectData* areaEffect;
    LogicSkillData* weaponSkill;
    LogicSkillData* ultimateSkill;
    bool secondaryPet;
    bool isExternalPet;
    bool shouldEncodePetStatus;
    int powerLevelsType;
    bool carryableType;
    int petAutoSpawnDelay;
    LogicCharacterData* pet;

    bool isHero();
    bool hasAutoAttack();
    int getSpeed();
    int getUniqueProperty();
    int getUniquePropertyValue1();
    int getUniquePropertyValue2();
    bool isTrainOrPayload();
    int getLifeTimeTicks();
    bool isDecoy();
    bool canGainShield();
    bool isMinionTwin();
    bool isDog();
    bool isCarryable();
    LogicCharacterData* getSpawnedPet();
    bool isMinionLastStand();
    bool isBoss();

    bool isCharacter();
};