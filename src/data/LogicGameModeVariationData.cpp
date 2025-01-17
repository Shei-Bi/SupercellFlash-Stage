#include "LogicGameModeVariationData.h"
#include "csv/CSVTable.h"
LogicGameModeVariationData::LogicGameModeVariationData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicGameModeVariationData::resetStatics(CSVTable* table) {
    sm_columnIndexVariation = table->getColumnIndexByName("Variation");
    sm_columnIndexGameModeIconName = table->getColumnIndexByName("GameModeIconName");
    sm_columnIndexHasTimerAndCanEndBeforeTimerRunsOut = table->getColumnIndexByName("HasTimerAndCanEndBeforeTimerRunsOut");
    sm_columnIndexRoundResetsWhenObjectiveIsMissing = table->getColumnIndexByName("RoundResetsWhenObjectiveIsMissing");
    sm_columnIndexHasTwoBases = table->getColumnIndexByName("HasTwoBases");
    sm_columnIndexIsTypeOfBossFight = table->getColumnIndexByName("IsTypeOfBossFight");
    sm_columnIndexTeamSize = table->getColumnIndexByName("TeamSize");
    sm_columnIndexTeamCount = table->getColumnIndexByName("TeamCount");
    sm_columnIndexPlayerCollectBountyStars = table->getColumnIndexByName("PlayerCollectBountyStars");
    sm_columnIndexModeHasCarryables = table->getColumnIndexByName("ModeHasCarryables");
}
int LogicGameModeVariationData::getVariation() {
    return csvRow->getIntegerValueAt(sm_columnIndexVariation);
}
const std::string& LogicGameModeVariationData::getGameModeIconName() {
    return csvRow->getValueAt(sm_columnIndexGameModeIconName);
}
bool LogicGameModeVariationData::hasTimerAndCanEndBeforeTimerRunsOut() {
    return csvRow->getBooleanValueAt(sm_columnIndexHasTimerAndCanEndBeforeTimerRunsOut);
}
bool LogicGameModeVariationData::roundResetsWhenObjectiveIsMissing() {
    return csvRow->getBooleanValueAt(sm_columnIndexRoundResetsWhenObjectiveIsMissing);
}
bool LogicGameModeVariationData::hasTwoBases() {
    return csvRow->getBooleanValueAt(sm_columnIndexHasTwoBases);
}
bool LogicGameModeVariationData::isTypeOfBossFight() {
    return csvRow->getBooleanValueAt(sm_columnIndexIsTypeOfBossFight);
}
int LogicGameModeVariationData::getPlayerCount() {
    return csvRow->getIntegerValueAt(sm_columnIndexTeamCount) * csvRow->getIntegerValueAt(sm_columnIndexTeamSize);
}
void LogicGameModeVariationData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    assert(getInstanceID() == csvRow->getIntegerValueAt(sm_columnIndexVariation));
}
bool LogicGameModeVariationData::isPlayerCollectBountyStars() {
    return csvRow->getBooleanValueAt(sm_columnIndexPlayerCollectBountyStars);
}

bool LogicGameModeVariationData::modeHasCarryables() {
    return csvRow->getBooleanValueAt(sm_columnIndexModeHasCarryables);
}
