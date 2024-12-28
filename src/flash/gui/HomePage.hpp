#pragma once
#include <flash/gui/DropGUIContainer.hpp>
#include <vector>
#include <flash/gui/PendingTeamItem.hpp>

#pragma optimize("",off);
class HomePage :public DropGUIContainer {
public:
    std::vector<MovieClip*> screenContainers;
    std::vector<GameButton*> buttons;

    PendingTeamItem* panel_own_invite_2;
    PendingTeamItem* panel_own_invite_4;
    PendingTeamItem* panel_own_invite_3;
    PendingTeamItem* panel_own_invite_5;
    PendingTeamItem* panel_other_invite_2;
    PendingTeamItem* panel_other_invite_4;
    PendingTeamItem* panel_other_invite_3;
    PendingTeamItem* panel_other_invite_5;
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
        getClipFromContainers("player_1_area")->visible = false;
        getClipFromContainers("player_2_area")->visible = false;
        getClipFromContainers("player_3_area")->visible = false;
        getClipFromContainers("player_4_area")->visible = false;
        getClipFromContainers("player_5_area")->visible = false;

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
        getButtonByName("button_mode")->visible = false;
    }

    void updateVisibleItems() {
        ;
    }

    void createButtons() {
        for (MovieClip* mc : screenContainers) {
            if (!mc) continue;
            mc->autoCreateButtons(buttons);
        }
    }

    GameButton* getButtonByName(const char* name) {
        for (GameButton* gameButton : buttons) {
            if (strcmp(gameButton->name, name) == 0) return gameButton;
        }
        return nullptr;
    }
};