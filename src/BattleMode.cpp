#include <BattleMode.h>
#include <ResourceManager.h>
#include <xTimer.h>
#include <GameStateManager.h>
#include "flash/gui/GUI.h"
#include <flash/gui/BattleScreen.hpp>
#include "MessageManager.h"
#include "battle/LogicBattleModeClient.h"
#include "battle/ClientInputManager.h"

BattleMode::BattleMode() {
    state = 0;
    resourcesToLoad = 0;
    battleScreen = nullptr;
    battleClient = nullptr;
    inputManager = new ClientInputManager();
}

BattleMode::~BattleMode() {
    if (battleScreen) {
        battleScreen->exit();
        delete battleScreen;
    }
    GUI::destructInstance();
}

bool BattleMode::isLoaded() {
    return state == 7;
}
bool BattleMode::isInited() {
    return battleClient && battleClient->canProceedToBattle();
}
void BattleMode::update(float sinceStart, float deltaTime) {
    battleClient->update(sinceStart, deltaTime);
    inputManager->update(sinceStart, deltaTime);
    battleScreen->update(deltaTime);
}
void BattleMode::updateLoading(float deltaTime) {
    switch (state) {
    case 0:
        state = 1;
        break;
    case 1:
    case 2:
        if (!battleClient || !battleClient->isStartLoadingMessageReceived()) return;
        state = 3;
    case 3:
        state = 4;
    case 4:
        if (loadNextResource()) state = 5;
        break;
    case 5:
        if (battleClient) {
            state = 6;
        }
        break;
    case 6:
        if (!MessageManager::getInstance()->isUdpConnectionInfoMessageReceived) return;
        GUI::constructInstance();
        GameMain::getInstance()->screen2->addChild(GUI::getInstance());
        battleScreen = new BattleScreen();
        battleScreen->enter();
        state = 7;
        break;
    }
}
float BattleMode::getLoadingProgress() {
    if (state == 7) return 1.0f;
    return 0.5f + 0.5f * (1.0f / 7.0f - ((int)ResourceManager::resourceToLoad() / 7.0f / resourcesToLoad) + state / 7.0f);
}
BattleMode* BattleMode::getInstance() {
    auto i = GameStateManager::getInstance();
    if (i->isState(GameStateManager::Battle)) return (BattleMode*)i->currentState;
    return nullptr;
}
void BattleMode::enter() {
    battleClient = new LogicBattleModeClient();
    // ResourceManager::addFile("sc/effects.sc");
    // ResourceManager::addFile("sc/effects_brawler.sc");
    ResourceManager::addFile("sc/ui.sc");
    ResourceManager::addFile("sc/characters.sc");
    ResourceManager::addFile("sc/level.sc");
    resourcesToLoad = ResourceManager::ResourcesToLoad.size();
}
bool BattleMode::loadNextResource() {
    unsigned long long NativeTime = xTimer::getNativeTime();
    while (ResourceManager::resourceToLoad() && xTimer::getPassedTimeMs(NativeTime, xTimer::getNativeTime()) < 100.0) {
        ResourceManager::loadNextResource();
    }
    return !ResourceManager::resourceToLoad();
}