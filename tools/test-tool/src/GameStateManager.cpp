#include <GameStateManager.h>
#include <LoadingScreen.h>
#include <InitState.h>
#include <LoadingScreen.h>
#include <HomeMode.h>

GameStateManager* GameStateManager::sm_pInstance = nullptr;
GameStateManager* GameStateManager::getInstance() {
    return sm_pInstance;
}
GameStateManager::GameStateManager() {
    loadingScreen = nullptr;
    currentState = nullptr;
    currentStateId = Null;
    pendingStateId = Null;
    gameDataLoaded = -1;
    home = nullptr;
    avatar = nullptr;
}
void GameStateManager::constructInstance()
{
    if (!GameStateManager::sm_pInstance)
        GameStateManager::sm_pInstance = new GameStateManager();
}
bool GameStateManager::hasGameData() {
    return gameDataLoaded != -1;
}
GameState* GameStateManager::createState(StateId id) {
    switch (id)
    {
    case Init:
        return new InitState();
    case Loading:
        return new LoadingScreen();
    case Home:
        return new HomeMode();
    }
    return nullptr;
}
void GameStateManager::setGameData(LogicClientHome* h, LogicClientAvatar* a) {
    home = h;
    avatar = a;
    gameDataLoaded = 1;
}
void GameStateManager::update(float sinceStart, float deltaTime) {
    if (pendingStateId) {
        currentState = createState(pendingStateId);
        currentState->enter();
        currentStateId = pendingStateId;
        pendingStateId = Null;
        deltaTime = 0.0f;
    }
    if (currentState) {
        if (currentState->isLoaded()) {
            currentState->update(sinceStart, deltaTime);
        }
        else {
            currentState->updateLoading(deltaTime);
            if (currentState->isLoaded()) {
                currentState->update(sinceStart, 0.0f);
            }
        }
    }
    if (currentState && (!currentState->isLoaded() || !currentState->isInited()) || pendingStateId) {
        if (!loadingScreen) {
            loadingScreen = createState(Loading);
            loadingScreen->enter();
        }
    }
    else {
        loadingScreen->setAlpha(0.0);
        return;
    }
    if (loadingScreen) {
        loadingScreen->setAlpha(1.0);
        loadingScreen->update(sinceStart, deltaTime);
    }
}
bool GameStateManager::isChangingMode() {
    return pendingStateId != Null;
}