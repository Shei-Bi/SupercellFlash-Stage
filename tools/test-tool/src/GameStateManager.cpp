#include <GameStateManager.h>
#include <LoadingScreen.h>
#include <InitState.h>
#include <LoadingScreen.h>

GameStateManager* GameStateManager::sm_pInstance = nullptr;
GameStateManager* GameStateManager::getInstance() {
    return sm_pInstance;
}
GameStateManager::GameStateManager() {
    loadingScreen = nullptr;
    currentState = nullptr;
    currentStateId = Null;
    pendingStateId = Null;
}
void GameStateManager::constructInstance()
{
    if (!GameStateManager::sm_pInstance)
        GameStateManager::sm_pInstance = new GameStateManager();
}
GameState* GameStateManager::createState(StateId id) {
    switch (id)
    {
    case Init:
        return new InitState();
    case Loading:
        return new LoadingScreen();
    }
    return nullptr;
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
    if (loadingScreen) {
        loadingScreen->setAlpha(1.0);
        loadingScreen->update(sinceStart, deltaTime);
    }
}