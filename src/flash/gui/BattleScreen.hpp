#pragma once
#include <flash/Stage.h>
#include <GameMain.h>
#include <flash/MovieClip.h>
#include <ResourceManager.h>
// #include "HomePage.h"
#include "GUI.h"
#include "CombatHUD.hpp"

#include <stdio.h>

class BattleScreen {
public:
    Sprite* idk;
    CombatHUD* combatHUD;
    BattleScreen() {
        idk = nullptr;
        combatHUD = nullptr;
    }
    ~BattleScreen() {
        if (idk) delete idk;
        if (combatHUD) delete combatHUD;
    }
    void exit() {
        ;
    }
    void enter() {
        combatHUD = new CombatHUD();
        idk = new Sprite();
        GameMain::getInstance()->screen2->addChildAt(idk, 0);
        GameMain::getInstance()->screen2->addChildAt(combatHUD, 1);
    }
    void openMatchMakingPopup() {
        // GUI::getInstance()->showPopup
    }
    void update(float deltaTime) {
        combatHUD->update(deltaTime);
    }
};