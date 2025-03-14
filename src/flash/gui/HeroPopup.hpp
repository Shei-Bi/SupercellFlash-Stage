#pragma once
#include <flash/gui/PopupBase.hpp>
#include "HeroList.hpp"

class HeroPopup :public PopupBase {
public:
    HeroList* heroList;
    std::vector<MovieClip*> screenContainers;
    HeroPopup() :PopupBase("sc/ui.sc", "screen_area", true, false, "", "", "") {
        setX(Stage::getInstance()->matrixX * 0.5f);
        setUpScreenHeader();

        heroList = new HeroList();
        addChild(heroList);

        // getMovieClip()->initScreenContainers("brawler_list_screen_", screenContainers);

        // screenContainers[6]->setX(Stage::getInstance()->matrixX * 0.5f);
    }
    void refresh() {
        ;
    }
    void buttonClicked(GameButton* button) {
        if (button->name == "button_home" || button->name == "button_back") {
            delete this;
        }
    }
};