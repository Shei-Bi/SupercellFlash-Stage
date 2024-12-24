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
};