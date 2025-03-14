#pragma once
#include <flash/gui/GenericPopup.hpp>

class SelectLocationPopup :public GenericPopup {
public:
    SelectLocationPopup() :GenericPopup("shop_page_hor", true, false, "", "", "") {
        setUpScreenHeader();
        movieClip->setX(Stage::getInstance()->matrixX * 0.5f);
    }
    void buttonClicked(GameButton* button) {
        GenericPopup::buttonClicked(button);
    }

};