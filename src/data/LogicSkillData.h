#pragma once
#include "LogicData.h"
#include "LogicProjectileData.h"

class CSVTable;
enum BehaviorType {
    Attack,
    Blink,
    Charge,
    Invisibility,
    TransformMinion,
    BuffMinion,
    Rage,
    Shield,
    Ww,
    AreaBuff,
    ChangeProjectile,
    ChangeCharacter,
    ChangeWeapon,
    RecallWeapon,
    ProjectileShield,
    Heal,
    Portals,
    DeathRay,
    Split,
    FlyJump,
    MountUp,
    Teleport
};

class LogicSkillData :public LogicData {
public:
    static inline int sm_columnIndexChargedShotCount = -1;
    static inline int sm_columnIndexAttackPattern = -1;
    static inline int sm_columnIndexProjectile = -1;
    static inline int sm_columnIndexSkillChangeType = -1;
    static inline int sm_columnIndexHoldToShoot = -1;
    static inline int sm_columnIndexMaxCharge = -1;
    static inline int sm_columnIndexChargeType = -1;
    static inline int sm_columnIndexBehaviorType = -1;

    LogicSkillData(CSVRow* csvRow, LogicDataTable* parentTable);
    void createReferences();
    static void resetStatics(CSVTable*);

    LogicProjectileData* projectile;
    int behaviorType;

    int getChargedShotCount();
    int getAttackPattern();
    int getSkillChangeType();
    bool isHoldToShoot();
    int getMaxCharge();
    int getChargeType();
};