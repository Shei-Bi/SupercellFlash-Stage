#include <InitState.h>
#include <GameStateManager.h>
#include <ServerConnection.h>
#include "data/LogicDataTables.h"

void InitState::enter() {
    state = 1;
}
void InitState::update(float sinceStart, float deltaTime) {
    if (isLoaded()) GameStateManager::getInstance()->pendingStateId = GameStateManager::Home;
}
void InitState::updateLoading(float deltaTime) {
    switch (state) {
    case 1:
        initDataTables();
        state = 2;
        break;
    }
}
void InitState::initDataTables() {
    if (!LogicDataTables::isLoaded()) {
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/location_themes.csv"), LocationTheme);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/locations.csv"), Location);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/game_mode_variations.csv"), GameModeVariation);
        LogicDataTables::createReferences();
    }
}
bool InitState::isLoaded() {
    return state == 2 && ServerConnection::getInstance()->state == ServerConnection::Logined;
}
float InitState::getLoadingProgress() {
    return 0.2f;
}