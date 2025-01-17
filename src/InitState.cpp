#include <InitState.h>
#include <GameStateManager.h>
#include <ServerConnection.h>
#include "data/LogicDataTables.h"
#include "csv/CSVNode.hpp"

void InitState::enter() {
    state = 1;
}
void InitState::update(float sinceStart, float deltaTime) {
    if (isLoaded()) GameStateManager::getInstance()->changeState(GameStateManager::Home);
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
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/characters.csv"), Character);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/skins.csv"), Skin);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/location_themes.csv"), LocationTheme);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/locations.csv"), Location);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/game_mode_variations.csv"), GameModeVariation);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/cards.csv"), Card);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/accessories.csv"), Accessory);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/gear_boosts.csv"), GearBoost);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/maps.csv"), Map);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/tiles.csv"), Tile);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/projectiles.csv"), Projectile);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/gear_boosts.csv"), GearBoost);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/area_effects.csv"), AreaEffect);
        LogicDataTables::initDataTable(CSVNode::fromFile("assets/csv_logic/skills.csv"), Skill);
        LogicDataTables::createReferences();
    }
}
bool InitState::isLoaded() {
    return state == 2 && ServerConnection::getInstance()->state == ServerConnection::Logined;
}
float InitState::getLoadingProgress() {
    return 0.2f;
}