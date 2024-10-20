#include <InitState.h>
#include <GameStateManager.h>

void InitState::update(float sinceStart, float deltaTime) {
    if (isLoaded()) GameStateManager::getInstance()->pendingStateId = GameStateManager::Home;
}
bool InitState::isLoaded() {
    return false;
}
float InitState::getLoadingProgress() {
    return 0.2f;
}