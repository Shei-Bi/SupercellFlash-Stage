#pragma once
#include <flash/Stage.h>
#include <GameMain.h>
#include <flash/MovieClip.h>
#include <ResourceManager.h>
#include "HomePage.h"
#include "GUI.h"

#include <stdio.h>

class HomeScreen {
public:
    MovieClip* background;
    HomePage* homePage;

    void enter() {
        Stage* Stage = Stage::getInstance();
        Sprite* screen1 = GameMain::getInstance()->screen1;
        ResourceManager::addFile("sc/background_toystory.sc");
        ResourceManager::loadNextResource();
        background = ResourceManager::getMovieClip("sc/background_toystory.sc", "bgr_toystory");
        // printf("%d\n", ResourceManager::getSupercellSWF("sc/background_vp.sc", "bgr_vp")->textures[1].pixel_format());

        screen1->addChild(background);

        MovieClip* bg_colour = background->getMovieClipByName("bg_colour");
        // bg_colour->setAlpha(0.0f);
        if (!bg_colour) bg_colour = background;
        float scaleX = (Stage->matrixX + 4) / bg_colour->getWidth();
        float scaleY = (Stage->matrixY + 4) / bg_colour->getHeight();
        if (scaleX > 1.0 || scaleY > 1.0) bg_colour->setScale(fmax(scaleX, scaleY));
        background->setXY(Stage->matrixX / 2, Stage->matrixY / 2);

        homePage = new HomePage();
        screen1->addChild(homePage);
    }
    void openMatchMakingPopup() {
        // GUI::getInstance()->showPopup
    }
    void update(float deltaTime) {
        homePage->update(deltaTime);
    }
    void exit() {
        if (background) delete background;
        if (homePage) delete homePage;
    }
};