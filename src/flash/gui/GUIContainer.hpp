#pragma once
#include <flash/Sprite.h>
#include <ResourceManager.h>
#include <flash/MovieClip.h>

class GUIContainer :public Sprite {
public:
    MovieClip* movieClip;
    GUIContainer(char* fileName, char* exportName) :Sprite(1) {
        movieClip = nullptr;
        setMovieClip(ResourceManager::getMovieClip(fileName, exportName));
    }

    GUIContainer() :Sprite(1) {
        movieClip = nullptr;
    }

    virtual void setMovieClip(MovieClip* movieClip) {
        if (this->movieClip && this->movieClip != movieClip) delete this->movieClip;
        this->movieClip = movieClip;
        addChild(movieClip);
    }
    MovieClip* getMovieClip() {
        return movieClip;
    }
};