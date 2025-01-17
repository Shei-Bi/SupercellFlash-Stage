#include <GameStateManager.h>
#include <LoadingScreen.h>
#include <InitState.h>
#include <LoadingScreen.h>
#include <HomeMode.h>
#include "BattleMode.h"
#include "GameMain.h"

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
    if (!GameStateManager::sm_pInstance) {
        GameStateManager::sm_pInstance = new GameStateManager();
        GameMain::getInstance()->inputSystem->listeners.push_back(GameStateManager::sm_pInstance);
    }
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
    case Battle:
        return new BattleMode();
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
void GameStateManager::changeState(StateId id) {
    pendingStateId = id;
}
void GameStateManager::changeToState() {
    exitAndDestroyState();
    currentState = createState(pendingStateId);
    currentState->enter();
    currentStateId = pendingStateId;
    pendingStateId = Null;
}
void GameStateManager::exitAndDestroyState() {
    if (!currentState) return;
    currentState->exit();
    delete currentState;
    currentState = nullptr;
}
bool GameStateManager::touchPressed(Touch& t) {
    if (currentState) return currentState->touchPressed(t);
    return false;
}
bool GameStateManager::touchMoved(Touch& t) {
    if (currentState) return currentState->touchMoved(t);
    return false;
}
bool GameStateManager::touchReleased(Touch& t) {
    if (currentState) return currentState->touchReleased(t);
    return false;
}
void GameStateManager::update(float sinceStart, float deltaTime) {
    if (pendingStateId) {
        changeToState();
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