#pragma once
#include <GameMain.h>
#include <flash/Stage.h>
#include <ResourceManager.h>
#include <GameStateManager.h>
#include <ServerConnection.h>

GameMain* GameMain::sm_pInstance = nullptr;
GameMain* GameMain::getInstance() {
    return sm_pInstance;
}
void GameMain::constructInstance()
{
    if (!GameMain::sm_pInstance)
        GameMain::sm_pInstance = new GameMain();
}
void GameMain::init() {
    ResourceManager::init();
    Stage::constructInstance();
    GameStateManager::constructInstance();
    ServerConnection::constructInstance();
    GameStateManager::getInstance()->pendingStateId = GameStateManager::Init;
    screenSprite = new Sprite(1);
    Stage::getInstance()->addChild(screenSprite);
    loadingScreenSprite = new Sprite(1);
    Stage::getInstance()->addChild(loadingScreenSprite);
    Stage::getInstance()->setBackgroundColor(0);
}
void GameMain::update(float sinceStart, float deltaTime) {
    this->deltaTime = deltaTime;
    GameStateManager::getInstance()->update(sinceStart, deltaTime);
    ServerConnection::getInstance()->update(deltaTime);
}
void GameMain::draw(float e) {
    Stage::getInstance()->render(deltaTime, true);
}
GameMain::GameMain() {
    ;
}