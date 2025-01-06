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
#include "HomeMode.h"

// #pragma optimize("",off)
class HomePage :public DropGUIContainer {
public:
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
    HomePage() : DropGUIContainer("sc/ui.sc", "screen_area") {
        getMovieClip()->initScreenContainers("mainscreen_", screenContainers);
        // return;
        MovieClip* header_bgr = getClipFromContainers("header_bgr");
        if (header_bgr) {
            float scaled = Stage::getInstance()->matrixX / header_bgr->getWidth();
            if (scaled > 1.0)
                header_bgr->setScaleX(header_bgr->getScaleX() * scaled);
        }
        screenContainers[1]->gotoAndStop("3_players");
        panel_own_invite_2 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_2"));
        screenContainers[1]->changeTimelineChild("panel_own_invite_2", panel_own_invite_2);
        panel_other_invite_2 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_2"));
        screenContainers[1]->changeTimelineChild("panel_other_invite_2", panel_other_invite_2);
        panel_own_invite_4 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_4"));
        screenContainers[1]->changeTimelineChild("panel_own_invite_4", panel_own_invite_4);
        panel_other_invite_4 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_4"));
        screenContainers[1]->changeTimelineChild("panel_other_invite_4", panel_other_invite_4);
        panel_own_invite_3 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_3"));
        screenContainers[1]->changeTimelineChild("panel_own_invite_3", panel_own_invite_3);
        panel_other_invite_3 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_3"));
        screenContainers[1]->changeTimelineChild("panel_other_invite_3", panel_other_invite_3);
        panel_own_invite_5 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_5"));
        screenContainers[1]->changeTimelineChild("panel_own_invite_5", panel_own_invite_5);
        panel_other_invite_5 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_5"));
        screenContainers[1]->changeTimelineChild("panel_other_invite_5", panel_other_invite_5);
        panel_own_invite_2->visible = false;
        panel_own_invite_4->visible = false;
        panel_own_invite_3->visible = false;
        panel_own_invite_5->visible = false;
        panel_other_invite_2->visible = false;
        panel_other_invite_4->visible = false;
        panel_other_invite_3->visible = false;
        panel_other_invite_5->visible = false;

        createButtons();
        getButtonByName("button_navi_login_calendar")->visible = false;
        // screenContainers[5]->moveThisToTopLayer();
        // screenContainers[9]->moveThisToTopLayer();
        // auto player_1_area = getClipFromContainers("player_1_area");
        // player_1_area->stop();
        // player_1_area->setChildVisible("bubble", false);
        // player_1_area->setChildVisible("player_status", false);
        // player_1_area->setChildVisible("star_power_ph", false);
        // player_1_area->setChildVisible("item_ph", false);
        button_play_club_league = getButtonByName("button_play_club_league");
        getClipFromContainers("player_1_area")->visible = false;
        getClipFromContainers("player_2_area")->visible = false;
        getClipFromContainers("player_3_area")->visible = false;
        getClipFromContainers("player_4_area")->visible = false;
        getClipFromContainers("player_5_area")->visible = false;
        brawl_container = screenContainers[4]->getMovieClipByName("brawl_container");
        // brawl_container->visible = false;

        getButtonByName("button_random_reward")->visible = false;


        screenContainers[6]->getMovieClipByName("hamburger_menu")->stop();


        screenContainers[1]->setChildVisible("panel_player_online", false);
        screenContainers[1]->setChildVisible("panel_player_online_ph_left", false);
        screenContainers[1]->setChildVisible("panel_player_online_ph_right", false);
        screenContainers[1]->setChildVisible("panel_player_online_ph_left_left", false);
        screenContainers[1]->setChildVisible("panel_player_online_right_right", false);

        refreshSelectedEvent();
    }

    MovieClip* getClipFromContainers(const char* name) {
        for (MovieClip* mc : screenContainers) {
            if (!mc) continue;
            MovieClip* result = mc->getMovieClipByName(name);
            if (result) return result;
        }
        return nullptr;
    }

    void onInitialized() {
        displayEventList();
    }

    void displayEventList() {

    }

    void deleteAllPages() {
        ;
    }

    void refreshSelectedEvent() {
        selectedEvent = nullptr;
        int availableslot = 0;
        while (selectedEvent == nullptr)
            selectedEvent = GameStateManager::getInstance()->home->getActiveEventForSlot(availableslot++);
        selectedEvent = GameStateManager::getInstance()->home->getFirstEventOfGMV(20);
        auto button_mode_clip = getButtonByName("button_mode")->timelineClip;
        // button_mode_clip->debugPrintChildNames();
        // button_mode_clip->getMovieClipByName("gamemode")->setAlpha(0.5f);
        auto info = button_mode_clip->getMovieClipByName("info");
        // info->setAlpha(0.1f);

        brawl_container->getMovieClipByName("party_mode_container")->visible = false;
        getClipFromContainers("raid_boss_container")->visible = false;

        button_play_club_league->visible = false;
        // brawl_container->visible = false;//?????????//

        getButtonByName("button_spectate")->visible = false;

        // button_mode_clip->setChildVisible("notification", false);

        button_mode_clip->gotoAndStop("idle");
        auto event_container = getClipFromContainers("event_container");

        event_container->getMovieClipByName("recommended_brawlers_anim")->stop();

        info->setChildVisible("icon_brawler", false);

        gamemode_icon = new DataIcon(nullptr);
        gamemode_icon->setIconClip(std::string("sc/ui.sc"), LogicDataTables::getGameModeVariationData(6)->getGameModeIconName());
        gamemode_icon->replaceInstanceWithIcon(info->getMovieClipByName("gamemode_icon"), "icon", 1, 1);
        info->getMovieClipByName("gamemode_icon")->setChildVisible("icon", false);
        // info->debugForceNewBucket = true;
        // gamemode_icon->setScale(10.0f);
        // info->moveThisToTopLayer();
        // info->moveThisToTopLayer();


        // for (int i = 0;i < event_container->timelineChildrenCount;i++) {
        //     printf("%s\n", event_container->childrenNames[i]);
        // }
        event_container->setChildVisible("mutant_ph", false);

        info->playOnce();

        refreshRankedElement();
        getButtonByName("button_quest_icon")->visible = false;
        button_mode_clip->setChildVisible("pro_league", false);
        refreshSelectedEventChampionshipChallenge();
        refreshSelectedEventSeasonal();

    }

    void refreshRankedElement() {
        setUpRankedModeElementForEvent((MovieClip*)getClipFromContainers("rank_mode_label")->parent, nullptr);
    }
    static void setUpRankedModeElementForEvent(MovieClip* movieClip, EventData*) {
        movieClip->setChildVisible("rank_mode_label", false);
    }
    void refreshSelectedEventSeasonal() {
        getButtonByName("button_mode")->timelineClip->setChildVisible("halloween", false);
    }

    void refreshSelectedEventChampionshipChallenge() {
        // getButtonByName("button_mode")->timelineMovieClip->setChildVisible("championship_challenge", false);
    }

    void updateVisibleItems() {
        ;
    }

    void createButtons() {
        for (MovieClip* mc : screenContainers) {
            if (!mc) continue;
            mc->autoCreateButtons(buttons, this);
        }
    }

    void update(float deltaTime) {
        auto event_container = getClipFromContainers("event_container");
        event_container->getChildByName("vfx_overcharge")->visible = false;
        event_container->getChildByName("vfx_overcharge_front")->visible = false;

        //dead buttons club
        getButtonByName("button_pro_league")->visible = false;
        getButtonByName("button_ranked")->visible = false;

        getButtonByName("button_championship_challenge")->visible = false;
    }

    void handleModeButtonPress() {
        GUI::getInstance()->showPopup(new SelectLocationPopup());
    }
    void openMatchMakingPopup() {
        visible = false;
    }
    void buttonClicked(GameButton* button) {
        if (strcmp(button->name, "button_mode") == 0) {
            handleModeButtonPress();
        }
        else if (strcmp(button->name, "brawl_button") == 0) {
            HomeMode::getInstance()->getHomeScreen()->openMatchMakingPopup();
            MessageManager::getInstance()->messaging->send(new MatchmakeRequestMessage(selectedEvent->id, selectedEvent->slot));
        }
    }
};