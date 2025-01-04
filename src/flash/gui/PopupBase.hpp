#pragma once
#include <flash/gui/DropGUIContainer.hpp>
#include "flash/Stage.h"
#include "GUI.h"

class PopupBase :public DropGUIContainer {
    bool add_ui;
    bool use_bg_anim_popup;
    MovieClip* bg_clip;
    MovieClip* header_clip;//208
    const char* header_export_name;

public:
    PopupBase(const char* fileName, const char* exportName, bool add_ui, bool use_bg_anim_popup, const char* bg_file, const char* bg_export, const char* mheader_export_name) :DropGUIContainer(fileName, exportName), add_ui(add_ui), use_bg_anim_popup(use_bg_anim_popup), header_export_name(mheader_export_name) {
        if (header_export_name == nullptr || strlen(header_export_name) == 0)
            header_export_name = "screen_header";
        bg_clip = nullptr;
        header_clip = nullptr;
        init();
    }
    void init() {
        if (movieClip) {
            // if (use_bg_anim_popup)
            bg_clip = ResourceManager::getMovieClip("sc/ui.sc", "bg_anim_skulls");
            bg_clip->interactable = true;
            movieClip->addChildAt(bg_clip, 0);

            auto Stage = Stage::getInstance();
            bg_clip->setY(Stage->matrixY * 0.5f);
            auto bg_colour = bg_clip->getMovieClipByName("bg_colour");
            if (!bg_colour) bg_colour = bg_clip;
            //from HomePage::HomePage
            float scaleX = (Stage->matrixX + 4) / bg_colour->getWidth();
            float scaleY = (Stage->matrixY + 4) / bg_colour->getHeight();
            if (scaleX > 1.0 || scaleY > 1.0) bg_colour->setScale(fmax(scaleX, scaleY));
        }
        idk1();
    }

    void idk1() {
        if (!header_clip) {
            header_clip = ResourceManager::getMovieClip("sc/ui.sc", header_export_name);
            if (strcmp(header_export_name, "screen_header") == 0) {
                header_clip->setChildVisible("fame_tier_ph", false);
                header_clip->setChildVisible("tabs", false);
                header_clip->setChildVisible("button_trophy_road", false);
                header_clip->setChildVisible("button_season_box", false);
            }
            (movieClip ? movieClip : (MovieClip*)this)->addChild(header_clip);
            header_clip->setInteractiveRecursive(false);
            header_clip->interactable = true;
            header_clip->visible = false;
            header_clip->setChildVisible("header_brawlers", false);
        }
    }
    void setUpScreenHeader() {
        if (!header_clip) return;
        if (header_clip->getMovieClipByName("button_home")) {
            addGameButton(header_clip, "button_home");
        }
        if (header_clip->getMovieClipByName("button_back")) {
            addGameButton(header_clip, "button_back");
        }
        // header_clip->autoCreateButtons(buttons, this);

        GUI::moveToScreenRight(getButtonByName("button_home"));
        GUI::moveToScreenLeft(getButtonByName("button_back"));
        if (header_clip->getMovieClipByName("title_shop_txt"))
            GUI::moveToScreenLeft(header_clip->getMovieClipByName("title_shop_txt"));
        GUI::scaleXToScreenWidth(header_clip, "header_bgr", true);

        header_clip->visible = true;
        // movieClip->debugPrintChildNames();
        movieClip->setChildVisibleDontCrashIfNotFound("close_button", false);
        movieClip->setChildVisibleDontCrashIfNotFound("back_button", false);
    }
    bool touchPressed(Touch& t) {
        Sprite::touchPressed(t);
        return true;
    }
    bool touchReleased(Touch& t) {
        Sprite::touchReleased(t);
        return true;
    }
};