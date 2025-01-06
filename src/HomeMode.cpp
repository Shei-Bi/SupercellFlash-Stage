#include<HomeMode.h>
#include <ResourceManager.h>
#include <xTimer.h>
#include <GameStateManager.h>
#include "flash/gui/GUI.h"

bool HomeMode::isLoaded() {
    return state == 7;
}
void HomeMode::update(float sinceStart, float deltaTime) {
    homeScreen->update(deltaTime);
}
void HomeMode::updateLoading(float deltaTime) {
    switch (state) {
    case 0:
        state = 4;
        break;
    case 4:
        if (loadNextResource()) state = 5;
        break;
    case 5:
        if (GameStateManager::getInstance()->hasGameData()) {
            state = 6;
        }
        break;
    case 6:
        GUI::constructInstance();
        GameMain::getInstance()->screen2->addChild(GUI::getInstance());
        homeScreen = new HomeScreen();
        homeScreen->enter();
        state = 7;
        break;
    }
}
float HomeMode::getLoadingProgress() {
    if (state == 7) return 1.0f;
    return 0.5f + 0.5f * (1.0f / 7.0f - ((int)ResourceManager::resourceToLoad() / 7.0f / resourcesToLoad) + state / 7.0f);
}
HomeMode* HomeMode::getInstance() {
    auto i = GameStateManager::getInstance();
    if (i->isState(GameStateManager::Home)) return (HomeMode*)i->currentState;
    return nullptr;
}
HomeScreen* HomeMode::getHomeScreen() {
    return homeScreen;
}
void HomeMode::enter() {
    // ResourceManager::addFile("sc/effects.sc");
    // ResourceManager::addFile("sc/effects_brawler.sc");
    ResourceManager::addFile("sc/ui.sc");
    ResourceManager::addFile("sc/characters.sc");
    ResourceManager::addFile("sc/level.sc");
    resourcesToLoad = ResourceManager::ResourcesToLoad.size();
}
bool HomeMode::loadNextResource() {
    unsigned long long NativeTime = xTimer::getNativeTime();
    while (ResourceManager::resourceToLoad() && xTimer::getPassedTimeMs(NativeTime, xTimer::getNativeTime()) < 100.0) {
        ResourceManager::loadNextResource();
    }
    return !ResourceManager::resourceToLoad();
}