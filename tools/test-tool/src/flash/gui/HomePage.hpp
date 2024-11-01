#pragma once
#include <flash/gui/DropGUIContainer.hpp>
#include <vector>
#include <flash/gui/PendingTeamItem.hpp>

class HomePage :public DropGUIContainer {
public:
    std::vector<MovieClip*> screenContainers;
    PendingTeamItem* panel_invite;
    PendingTeamItem* panel_invite_2;
    PendingTeamItem* panel_invite_3;
    PendingTeamItem* panel_invite_4;

    HomePage() : DropGUIContainer("sc/ui.sc", "screen_area") {
        getMovieClip()->initScreenContainers("mainscreen_", screenContainers);

        MovieClip* header_bgr = getClipFromContainers("header_bgr");
        if (header_bgr) {
            float scaled = Stage::getInstance()->matrixX / header_bgr->getWidth();
            if (scaled > 1.0)
                header_bgr->setScaleX(header_bgr->getScaleX() * scaled);
        }

        panel_invite = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_invite"));
        panel_invite_2 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_invite_2"));
        panel_invite_3 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_invite_3"));
        panel_invite_4 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_invite_4"));
        screenContainers[1]->addChild(panel_invite);
        screenContainers[1]->addChild(panel_invite_2);
        screenContainers[1]->addChild(panel_invite_3);
        screenContainers[1]->addChild(panel_invite_4);
        panel_invite->visible = false;
        panel_invite_2->visible = false;
        panel_invite_3->visible = false;
        panel_invite_4->visible = false;

        getClipFromContainers("player_1_area")->stop();
    }

    MovieClip* getClipFromContainers(char* name) {
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
        ;
    }

    void updateVisibleItems() {
        ;
    }
};