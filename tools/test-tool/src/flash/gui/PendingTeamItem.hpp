#pragma once
#include <flash/gui/DropGUIContainer.hpp>

class PendingTeamItem :public DropGUIContainer {
public:

    PendingTeamItem(MovieClip* movieClip) :DropGUIContainer(movieClip) {
        if (getMovieClip()->getMovieClipByName("button_cancel")) {
            ;
        }
    }
};