#pragma once
#include <GameMain.h>
#include <flash/Stage.h>

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
    Stage::constructInstance();

}
void GameMain::update(float sinceStart, float deltaTime) {
    this->deltaTime = deltaTime;
}
void GameMain::draw(float e) {
    Stage::getInstance()->render(deltaTime, true);
}
GameMain::GameMain() {
    ;
}