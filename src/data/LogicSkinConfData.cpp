#include "LogicSkinConfData.h"
#include "csv/CSVTable.h"
#include "LogicDataTables.h"

LogicSkinConfData::LogicSkinConfData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    character = nullptr;
}

void LogicSkinConfData::createReferences() {
    if (csvRow->rowIndex == 0) resetStatics(csvRow->table);
    character = LogicDataTables::getCharacterByName(csvRow->getValueAt(sm_columnIndexCharacter));
}

void LogicSkinConfData::resetStatics(CSVTable* table) {
    sm_columnIndexCharacter = table->getColumnIndexByName("Character");
    sm_columnIndexModel = table->getColumnIndexByName("Model");
    sm_columnIndexHappyAnim = table->getColumnIndexByName("HappyAnim");
    sm_columnIndexHappyFace = table->getColumnIndexByName("HappyFace");
    sm_columnIndexIdleAnim = table->getColumnIndexByName("IdleAnim");
    sm_columnIndexIdleFace = table->getColumnIndexByName("IdleFace");
    sm_columnIndexHappyLoopAnim = table->getColumnIndexByName("HappyLoopAnim");
    sm_columnIndexHappyLoopFace = table->getColumnIndexByName("HappyLoopFace");
    sm_columnIndexLobbyAnim = table->getColumnIndexByName("LobbyAnim");
    sm_columnIndexLobbyFace = table->getColumnIndexByName("LobbyFace");
    sm_columnIndexLobbyLoopAnim = table->getColumnIndexByName("LobbyLoopAnim");
    sm_columnIndexLobbyLoopFace = table->getColumnIndexByName("LobbyLoopFace");
}

std::string LogicSkinConfData::getModel() {
    return csvRow->getValueAt(sm_columnIndexModel);
}

const std::string LogicSkinConfData::getIdleAnim() {
    return csvRow->getValueAt(sm_columnIndexIdleAnim);
}

const std::string LogicSkinConfData::getIdleFace() {
    return csvRow->getValueAt(sm_columnIndexIdleFace);
}

const std::string LogicSkinConfData::getHappyAnim() {
    return csvRow->getValueAt(sm_columnIndexHappyAnim);
}

const std::string LogicSkinConfData::getHappyFace() {
    return csvRow->getValueAt(sm_columnIndexHappyFace);
}

const std::string LogicSkinConfData::getHappyLoopAnim() {
    return csvRow->getValueAt(sm_columnIndexHappyLoopAnim);
}

const std::string LogicSkinConfData::getHappyLoopFace() {
    return csvRow->getValueAt(sm_columnIndexHappyLoopFace);
}

const std::string LogicSkinConfData::getLobbyAnim() {
    return csvRow->getValueAt(sm_columnIndexLobbyAnim);
}

const std::string LogicSkinConfData::getLobbyFace() {
    return csvRow->getValueAt(sm_columnIndexLobbyFace);
}

const std::string LogicSkinConfData::getLobbyLoopAnim() {
    return csvRow->getValueAt(sm_columnIndexLobbyLoopAnim);
}

const std::string LogicSkinConfData::getLobbyLoopFace() {
    return csvRow->getValueAt(sm_columnIndexLobbyLoopFace);
}