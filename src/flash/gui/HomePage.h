#pragma once
#include <flash/gui/DropGUIContainer.hpp>
#include <vector>
#include <flash/gui/PendingTeamItem.hpp>
#include "flash/gui/DataIcon.hpp"
#include "data/LogicDataTables.h"
#include "logic/EventData.hpp"
#include "GUI.h"
#include "SelectLocationPopup.hpp"
#include "MessageManager.h"
#include "network/MatchmakeRequestMessage.hpp"
#include "logic/EventData.hpp"
#include "GameStateManager.h"
#include <HomeMode.h>
#include "network/SinglePlayerMatchRequestMessage.h"
#include "RotateHeroButton.hpp"

class HomePageTeamMember;
// #pragma optimize("",off)
class HomePage :public DropGUIContainer {
public:
    enum GameType {
        Trophies = 1,
        Training = 4
    };
    std::vector<MovieClip*> screenContainers;
    // std::vector<GameButton*> buttons;

    PendingTeamItem* panel_own_invite_2;
    PendingTeamItem* panel_own_invite_4;
    PendingTeamItem* panel_own_invite_3;
    PendingTeamItem* panel_own_invite_5;
    PendingTeamItem* panel_other_invite_2;
    PendingTeamItem* panel_other_invite_4;
    PendingTeamItem* panel_other_invite_3;
    PendingTeamItem* panel_other_invite_5;

    MovieClip* brawl_container;//696
    GameButton* button_play_club_league;//528
    DataIcon* gamemode_icon;//872

    EventData* selectedEvent;

    HomePageTeamMember* player1;
    RotateHeroButton* player1_button;

    HomePage();
    ~HomePage();

    MovieClip* getClipFromContainers(const char* name);
    void onInitialized();
    void displayEventList();
    void deleteAllPages();
    void refreshSelectedCharacters(bool);
    void refreshSelectedEvent();
    void refreshRankedElement();
    static void setUpRankedModeElementForEvent(MovieClip* movieClip, EventData*);
    void refreshSelectedEventSeasonal();
    void refreshSelectedEventChampionshipChallenge();
    void updateVisibleItems();
    void createButtons();
    void update(float deltaTime);
    void handleModeButtonPress();
    void buttonClicked(GameButton* button);
    void startGame(EventData* event, LogicData* location, int type, LogicCharacterData* character, std::vector<LogicCharacterData*>& characters);
    RotateHeroButton* addPlayerButton(MovieClip* movieClip, const char* name);
};