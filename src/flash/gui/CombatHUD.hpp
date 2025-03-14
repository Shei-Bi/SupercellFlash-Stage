#pragma once
#include "DropGUIContainer.hpp"
#include "flash/Stage.h"
#include "MessageManager.h"
#include "network/RequestMatchCancelMessage.h"
#include "BattleMode.h"
#include "battle/ClientInputManager.h"
#include "network/GoHomeMessage.h"
#include "battle/LogicBattleModeClient.h"

class CombatHUD :public Sprite, public IButtonListener {
public:
    std::vector<MovieClip*> screenContainers;
    GameButton* quit;
    float width, height;
    bool goHomeMessageSent;
    float timeSinceQuitButtonClicked;
    CombatHUD() :Sprite() {
        goHomeMessageSent = false;
        timeSinceQuitButtonClicked = 0.0f;
        interactable = true;
        Stage* Stage = Stage::getInstance();
        width = Stage->matrixX;
        height = Stage->matrixY;

        initScreenContainers("ingame_", screenContainers);
        addScreenContainer("ingame_bottom_right", 4, screenContainers);

        quit = new GameButton();
        quit->setMovieClip(getClipFromContainers("quit"), true);
        quit->setButtonListener(this);
        addChild(quit);
        quit->setPixelSnappedXY(width * 0.5f, height - 16.0f);
    }
    ~CombatHUD() {
        for (MovieClip* mc : screenContainers) {
            if (!mc) continue;
            delete mc;
            mc = nullptr;
        }
    }
    MovieClip* getClipFromContainers(const char* name) {
        for (MovieClip* mc : screenContainers) {
            if (!mc) continue;
            MovieClip* result = mc->getMovieClipByName(name);
            if (result) return result;
        }
        return nullptr;
    }
    void buttonClicked(GameButton* button) {
        if (button == quit) {
            if (timeSinceQuitButtonClicked <= 0.0f) {
                BattleMode::getInstance()->inputManager->addInput(new ClientInput(4));
                MessageManager::getInstance()->sendMessage(new RequestMatchCancelMessage());
                timeSinceQuitButtonClicked = 10.0f;
            }
        }
    }
    void sendGoHomeMessage(bool offline, bool spectate, bool const* playerMap) {
        if (!goHomeMessageSent) {
            goHomeMessageSent = true;
            GameStateManager::getInstance()->clearGameData();
            GameStateManager::getInstance()->changeState(GameStateManager::Home);
            if (MessageManager::getInstance()->udpSocket)
                MessageManager::getInstance()->udpSocket->close();
            MessageManager::getInstance()->sendMessage(new GoHomeMessage());
        }
    }
    void update(float deltaTime) {
        auto battleClient = BattleMode::getInstance()->battleClient;
        if (battleClient->roundState != -1) {
            if (timeSinceQuitButtonClicked > 0.0f) {
                sendGoHomeMessage(false, false, nullptr);
            }
        }
    }
};