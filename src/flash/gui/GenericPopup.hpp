#pragma once
#include <flash/gui/PopupBase.hpp>

class GenericPopup :public PopupBase {
public:
    GenericPopup(const char* exportName, bool add_ui, bool use_bg_anim_popup, const char* bg_file, const char* bg_export, const char* header_export_name) :PopupBase("sc/ui.sc", exportName, add_ui, use_bg_anim_popup, bg_file, bg_export, header_export_name) {
        ;
    }
    void setUpScreenHeader() {
        // movieClip->debugPrintChildNames();
        // if(!movieClip)
        PopupBase::setUpScreenHeader();
    }
    void createContainer(const char* name, int index) {
        ;
    }

    void buttonClicked(GameButton* button) {
        if (button->name == "button_home" || button->name == "button_back") {
            delete this;
        }
    }
};