#include "LogicCharacterData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

#define FastSerialize(x) if(Type==#x) type=x;
LogicCharacterData::LogicCharacterData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    type = -1;
}
void LogicCharacterData::resetStatics(CSVTable* table) {
    sm_columnIndexType = table->getColumnIndexByName("Type");
    sm_columnIndexDamage = table->getColumnIndexByName("AutoAttackDamage");
    sm_columnIndexSpeed = table->getColumnIndexByName("Speed");
    sm_columnIndexUniqueProperty = table->getColumnIndexByName("UniqueProperty");
    sm_columnIndexUniquePropertyValue1 = table->getColumnIndexByName("UniquePropertyValue1");
    sm_columnIndexUniquePropertyValue2 = table->getColumnIndexByName("UniquePropertyValue2");
    sm_columnIndexManualRotations = table->getColumnIndexByName("ManualRotations");
    sm_columnIndexAreaEffect = table->getColumnIndexByName("AreaEffect");
    sm_columnIndexLifeTimeTicks = table->getColumnIndexByName("LifeTimeTicks");
    sm_columnIndexSecondaryPet = table->getColumnIndexByName("SecondaryPet");
    sm_columnIndexIsExternalPet = table->getColumnIndexByName("IsExternalPet");
    sm_columnIndexWeaponSkill = table->getColumnIndexByName("WeaponSkill");
    sm_columnIndexUltimateSkill = table->getColumnIndexByName("UltimateSkill");
    sm_columnIndexShouldEncodePetStatus = table->getColumnIndexByName("ShouldEncodePetStatus");
    sm_columnIndexPowerLevelsType = table->getColumnIndexByName("PowerLevelsType");
    sm_columnIndexCarryableType = table->getColumnIndexByName("CarryableType");
    sm_columnIndexPetAutoSpawnDelay = table->getColumnIndexByName("PetAutoSpawnDelay");
    sm_columnIndexPet = table->getColumnIndexByName("Pet");
}
void LogicCharacterData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    auto Type = csvRow->getValueAt(sm_columnIndexType);

    FastSerialize(Hero);
    FastSerialize(Npc_Boss);
    FastSerialize(Minion_FollowOwner);
    FastSerialize(Minion_FindEnemies);
    FastSerialize(Minion_Building);
    FastSerialize(Campaign_DefendTarget);
    FastSerialize(Pvp_Base);
    FastSerialize(Minion_Building_charges_ulti);
    FastSerialize(Minion_Dog);
    FastSerialize(LootBox);
    FastSerialize(Minion_FindEnemies2);
    FastSerialize(RoboWars);
    FastSerialize(Train);
    FastSerialize(Minion_Mirage);
    FastSerialize(Npc_Boss_TownCrush);
    FastSerialize(Carryable);
    FastSerialize(Minion_Duplicate);
    FastSerialize(Payload);
    FastSerialize(Minion_Invasion);
    FastSerialize(Minion_LastStand);
    FastSerialize(Minion_Percenter);
    FastSerialize(Minion_Twin);
    FastSerialize(Minion_Critter);
    FastSerialize(Minion_Orbiting);
    FastSerialize(Her0);
    FastSerialize(M);

    if (type == -1) abort();

    hasManualRotations = csvRow->getBooleanValueAt(sm_columnIndexManualRotations);
    areaEffect = LogicDataTables::getAreaEffectByName(csvRow->getValueAt(sm_columnIndexAreaEffect));
    secondaryPet = csvRow->getBooleanValueAt(sm_columnIndexSecondaryPet);
    isExternalPet = csvRow->getBooleanValueAt(sm_columnIndexIsExternalPet);
    weaponSkill = LogicDataTables::getSkillByName(csvRow->getValueAt(sm_columnIndexWeaponSkill));
    ultimateSkill = LogicDataTables::getSkillByName(csvRow->getValueAt(sm_columnIndexUltimateSkill));
    shouldEncodePetStatus = csvRow->getBooleanValueAt(sm_columnIndexShouldEncodePetStatus);
    powerLevelsType = csvRow->getIntegerValueAt(sm_columnIndexPowerLevelsType);
    carryableType = csvRow->getValueAt(sm_columnIndexCarryableType).size() > 0;
    petAutoSpawnDelay = csvRow->getIntegerValueAt(sm_columnIndexPetAutoSpawnDelay);
    pet = LogicDataTables::getCharacterByName(csvRow->getValueAt(sm_columnIndexPet));
}
bool LogicCharacterData::isHero() {
    return type == Hero || type == Her0;
}
bool LogicCharacterData::hasAutoAttack() {
    return csvRow->getIntegerValueAt(sm_columnIndexDamage) > 0;
}
int LogicCharacterData::getSpeed() {
    return csvRow->getIntegerValueAt(sm_columnIndexSpeed);
}
int LogicCharacterData::getUniqueProperty() {
    return csvRow->getIntegerValueAt(sm_columnIndexUniqueProperty);
}
int LogicCharacterData::getUniquePropertyValue1() {
    return csvRow->getIntegerValueAt(sm_columnIndexUniquePropertyValue1);
}
int LogicCharacterData::getUniquePropertyValue2() {
    return csvRow->getIntegerValueAt(sm_columnIndexUniquePropertyValue2);
}
bool LogicCharacterData::isTrainOrPayload() {
    return type == Train || type == Payload;
}
int LogicCharacterData::getLifeTimeTicks() {
    return csvRow->getIntegerValueAt(sm_columnIndexLifeTimeTicks);
}
bool LogicCharacterData::isDecoy() {
    return type == Minion_Mirage;
}
bool LogicCharacterData::canGainShield() {
    return type == Minion_FindEnemies || type == Minion_Building || secondaryPet || isExternalPet;
}
bool LogicCharacterData::isMinionTwin() {
    return type == Minion_Twin;
}
bool LogicCharacterData::isDog() {
    return type == Minion_Dog;
}
bool LogicCharacterData::isCharacter() {
    return true;
}
bool LogicCharacterData::isCarryable() {
    return carryableType;
}
LogicCharacterData* LogicCharacterData::getSpawnedPet() {
    return pet;
}

bool LogicCharacterData::isMinionLastStand() {
    return type == Minion_LastStand;
}

bool LogicCharacterData::isBoss() {
    return type == Npc_Boss || type == Npc_Boss_TownCrush;
}

#undef FastSerialize