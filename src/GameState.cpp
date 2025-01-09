#include <GameState.h>

GameState::~GameState() {
    ;
}

float GameState::getLoadingProgress() {
    return 1.0f;
}
bool GameState::isInited() {
    return true;
}
bool GameState::isLoaded() {
    return true;
}
void GameState::setAlpha(float a) {
    // LOG("[WARNING] Setting alpha not supported for this game state!\n")
    ;
}
void GameState::update(float e, float deltaTime) {
    ;
}
void GameState::updateLoading(float deltaTime) {
    ;
}
void GameState::enter() {
    ;
}
void GameState::exit() {
    ;
}