#pragma once
// #include "flash/gui/GameButton.h"
class GameButton;
class IButtonListener {
public:
    virtual void buttonClicked(GameButton*) { ; }
};