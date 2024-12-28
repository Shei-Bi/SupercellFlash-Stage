#include "GameButton.h"
#include "stdlib.h"
#include "flash/MovieClip.h"
void GameButton::setMovieClip(MovieClip* movieClip, bool timeline) {
    setDisplayObject(movieClip, timeline);
}
void GameButton::setDisplayObject(DisplayObject* movieClip, bool timeline) {
    displayObject = movieClip;
    if (movieClip->isMovieClip()) timelineMovieClip = (MovieClip*)movieClip;
    if (timeline && false) {
        ;
    }
    addChild(displayObject);
    //int tempState = state;
    //state = -1;
    //setState(0);
    //updateHitArea();
    //setState(tempState);
}
void GameButton::destructClips() {
    if (displayObject != timelineMovieClip) {
        if (displayObject) delete displayObject;
        displayObject = nullptr;
    }
    if (timelineMovieClip) delete timelineMovieClip;
    timelineMovieClip = nullptr;
}
void GameButton::setTimelineMovieClip(MovieClip* movieClip) {
    if (displayObject == nullptr || movieClip == nullptr) abort();
    movieClip->Matrix = displayObject->Matrix;
    movieClip->colorTransform = displayObject->colorTransform;
    auto child = movieClip->children[0];
    movieClip->changeTimelineChild(child, displayObject);
    if (child) delete child;
    timelineMovieClip = movieClip;
}