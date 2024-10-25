#include <InitState.h>
#include <GameStateManager.h>
#include <ServerConnection.h>

void InitState::update(float sinceStart, float deltaTime) {
    if (isLoaded()) GameStateManager::getInstance()->pendingStateId = GameStateManager::Home;
}
bool InitState::isLoaded() {
    return ServerConnection::getInstance()->state == ServerConnection::Logined;
}
float InitState::getLoadingProgress() {
    return 0.2f;
}