#include "LogicSkillData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

#define FastSerialize(x) if(Type==#x) behaviorType=x;

LogicSkillData::LogicSkillData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    behaviorType = -1;
}

void LogicSkillData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    projectile = LogicDataTables::getProjectileByName(csvRow->getValueAt(sm_columnIndexProjectile));
    auto Type = csvRow->getValueAt(sm_columnIndexBehaviorType);
    FastSerialize(Attack);
    FastSerialize(Blink);
    FastSerialize(Charge);
    FastSerialize(Invisibility);
    FastSerialize(TransformMinion);
    FastSerialize(BuffMinion);
    FastSerialize(Rage);
    FastSerialize(Shield);
    FastSerialize(Ww);
    FastSerialize(AreaBuff);
    FastSerialize(ChangeProjectile);
    FastSerialize(ChangeCharacter);
    FastSerialize(ChangeWeapon);
    FastSerialize(RecallWeapon);
    FastSerialize(ProjectileShield);
    FastSerialize(Heal);
    FastSerialize(Portals);
    FastSerialize(DeathRay);
    FastSerialize(Split);
    FastSerialize(FlyJump);
    FastSerialize(MountUp);
    FastSerialize(Teleport);

    if (behaviorType == -1) abort();
}

void LogicSkillData::resetStatics(CSVTable* table) {
    sm_columnIndexChargedShotCount = table->getColumnIndexByName("ChargedShotCount");
    sm_columnIndexAttackPattern = table->getColumnIndexByName("AttackPattern");
    sm_columnIndexProjectile = table->getColumnIndexByName("Projectile");
    sm_columnIndexSkillChangeType = table->getColumnIndexByName("SkillChangeType");
    sm_columnIndexHoldToShoot = table->getColumnIndexByName("HoldToShoot");
    sm_columnIndexMaxCharge = table->getColumnIndexByName("MaxCharge");
    sm_columnIndexChargeType = table->getColumnIndexByName("ChargeType");
    sm_columnIndexBehaviorType = table->getColumnIndexByName("BehaviorType");
}

int LogicSkillData::getChargedShotCount() {
    return csvRow->getIntegerValueAt(sm_columnIndexChargedShotCount);
}

int LogicSkillData::getAttackPattern() {
    return csvRow->getIntegerValueAt(sm_columnIndexAttackPattern);
}

int LogicSkillData::getSkillChangeType() {
    return csvRow->getIntegerValueAt(sm_columnIndexSkillChangeType);
}

bool LogicSkillData::isHoldToShoot() {
    return csvRow->getBooleanValueAt(sm_columnIndexHoldToShoot);
}

int LogicSkillData::getMaxCharge() {
    return csvRow->getIntegerValueAt(sm_columnIndexMaxCharge);
}

int LogicSkillData::getChargeType() {
    return csvRow->getIntegerValueAt(sm_columnIndexChargeType);
}

#undef FastSerialize