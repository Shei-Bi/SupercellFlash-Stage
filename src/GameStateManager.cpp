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
    intervalTime = 0.0f;
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
bool GameStateManager::isState(StateId id) {
    return currentStateId == id;
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
            if (intervalTime >= 0.99f)
                currentState->updateLoading(deltaTime);
            if (currentState->isLoaded()) {
                currentState->update(sinceStart, 0.0f);
            }
        }
    }
    float clampedTime = fminf(deltaTime, 0.1f);
    if (currentState && (!currentState->isLoaded() || !currentState->isInited()) || pendingStateId) {
        intervalTime += clampedTime * 1.6667f;
        if (intervalTime > 1.0f) {
            intervalTime = 1.0f;
            if (!loadingScreen) goto LABEL_39;
            goto LABEL_32;
        }
    }
    else {
        intervalTime -= clampedTime * 1.6667f;
        if (intervalTime < 0.0f) {
            intervalTime = 0.0f;
            if (loadingScreen) {
            LABEL_34:
                loadingScreen->exit();
                delete loadingScreen;
                loadingScreen = nullptr;
            }
            return;
        }
    }
    if (intervalTime > 0.0f && !loadingScreen) {
    LABEL_39:
        loadingScreen = createState(Loading);
        loadingScreen->enter();
        intervalTime = 0.01f;
    }
LABEL_32:
    if (intervalTime < 0.01f) {
        if (loadingScreen) goto LABEL_34;
    }
    if (loadingScreen) {
        loadingScreen->setAlpha(intervalTime);
        loadingScreen->update(sinceStart, deltaTime);
    }
}
bool GameStateManager::isChangingMode() {
    return pendingStateId != Null;
}