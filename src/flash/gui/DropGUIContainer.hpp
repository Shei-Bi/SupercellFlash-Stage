#pragma once
#include <flash/gui/GUIContainer.hpp>

class DropGUIContainer :public GUIContainer {
public:
    DropGUIContainer(const char* fileName, const char* exportName) :GUIContainer(fileName, exportName) {
        putLocalizedTIDsToTextFields(getMovieClip());
    }

    DropGUIContainer(MovieClip* movieClip) :GUIContainer() {
        setMovieClip(movieClip);
        putLocalizedTIDsToTextFields(getMovieClip());
    }
    void setMovieClip(MovieClip* movieClip) {
        GUIContainer::setMovieClip(movieClip);
        putLocalizedTIDsToTextFields(movieClip);
    }
    void putLocalizedTIDsToTextFields(MovieClip*) {
        ;
    }
    GameButton* addGameButton(MovieClip* target, const char* name) {
        GameButton* gameButton = new GameButton();
        gameButton->name = name;
        auto child = target->getMovieClipByName(name);
        target->changeTimelineChild(child, gameButton);
        child->Matrix.reset();
        gameButton->setMovieClip(child, true);
        gameButton->setButtonListener(this);
        gameButton->setInteractiveRecursive(true);//???????????
        buttons.push_back(gameButton);
        return gameButton;
    }
};