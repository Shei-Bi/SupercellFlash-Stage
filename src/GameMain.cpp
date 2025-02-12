#pragma once
#include <GameMain.h>
#include <flash/Stage.h>
#include <ResourceManager.h>
#include <GameStateManager.h>
#include <ServerConnection.h>
#include "Trace.hpp"

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
    inputSystem = new InputSystem();
    GameStateManager::constructInstance();
    ServerConnection::constructInstance();
    GameStateManager::getInstance()->changeState(GameStateManager::Init);
    screen1 = new Sprite(1);
    screen1->interactable = true;
    Stage::getInstance()->addChild(screen1);
    screen2 = new Sprite(1);
    screen2->interactable = true;
    Stage::getInstance()->addChild(screen2);
    screen3 = new Sprite(1);
    screen3->interactable = true;
    Stage::getInstance()->addChild(screen3);
    screen4 = new Sprite(1);
    screen4->interactable = true;
    Stage::getInstance()->addChild(screen4);
    Stage::getInstance()->setBackgroundColor(0);
    inputSystem->listeners.push_back(Stage::getInstance());
}
void GameMain::update(float sinceStart, float deltaTime) {
    this->deltaTime = deltaTime;
    TRACE(GameStateManager::getInstance()->update(sinceStart, deltaTime));
    TRACE(ServerConnection::getInstance()->update(deltaTime));
    TRACE(inputSystem->update());
}
void GameMain::draw(float e) {
    TRACE(Stage::getInstance()->render(deltaTime, true));
}
GameMain::GameMain() {
    ;
}