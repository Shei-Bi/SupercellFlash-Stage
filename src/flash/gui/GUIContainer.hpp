#pragma once
#include <flash/Sprite.h>
#include <ResourceManager.h>
#include <flash/MovieClip.h>
#include "IButtonListener.h"

class GUIContainer :public Sprite, public IButtonListener {
public:
    MovieClip* movieClip;
    std::vector<GameButton*> buttons;
    GUIContainer(const char* fileName, const  char* exportName) :Sprite(1) {
        interactable = true;
        movieClip = nullptr;
        setMovieClip(ResourceManager::getMovieClip(fileName, exportName));
    }

    GUIContainer() :Sprite(1) {
        movieClip = nullptr;
    }

    virtual void setMovieClip(MovieClip* movieClip) {
        if (this->movieClip && this->movieClip != movieClip) delete this->movieClip;
        this->movieClip = movieClip;
        movieClip->interactable = true;
        addChild(movieClip);
    }
    MovieClip* getMovieClip() {
        return movieClip;
    }
    GameButton* getButtonByName(const char* name) {
        for (GameButton* gameButton : buttons) {
            if (strcmp(gameButton->name, name) == 0) return gameButton;
        }
        return nullptr;
    }
};