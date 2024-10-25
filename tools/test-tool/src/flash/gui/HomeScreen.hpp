#pragma once
#include <flash/Stage.h>
#include <GameMain.h>
#include <flash/MovieClip.h>
#include <ResourceManager.h>

class HomeScreen {
public:
    MovieClip* background;
    void enter() {
        Stage* Stage = Stage::getInstance();

        ResourceManager::addFile("sc/background_vp.sc");
        ResourceManager::loadNextResource();
        background = ResourceManager::getMovieClip("sc/background_vp.sc", "bgr_vp");

        GameMain::getInstance()->screenSprite->addChild(background);
        background->setXY(Stage->matrixX / 2, Stage->matrixY / 2);
    }
};