#pragma once
#include <flash/gui/PopupBase.hpp>

class WaitingPopup :public PopupBase {
public:
    GameButton* quit_button;
    WaitingPopup() :PopupBase("sc/ui.sc", "rank_matchmaking_popup", false, false, "", "", "") {
        setY(Stage::getInstance()->matrixY * 0.5f);
        quit_button = addGameButton(movieClip, "quit_button");
        quit_button->setY(quit_button->getY() + ((Stage::getInstance()->matrixY - 576.0f) * 0.5f));
        movieClip->getMovieClipByName("rank_animated_events")->setChildVisible("select_event_mode_glow", false);
        setPixelSnappedXY(Stage::getInstance()->matrixX * 0.5f, Stage::getInstance()->matrixY * 0.5f);
    }
    ~WaitingPopup() {
        if (quit_button) delete quit_button;
    }
    // void buttonClicked(GameButton* button) {
    //     GenericPopup::buttonClicked(button);
    // }

};