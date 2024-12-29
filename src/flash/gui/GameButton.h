#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include "flash/Sprite.h"
class MovieClip;
class GameButton :public Sprite {
public:
    GameButton();
    DisplayObject* displayObject;
    MovieClip* timelineMovieClip;
    const char* name;

    void setMovieClip(MovieClip*, bool);
    void setDisplayObject(DisplayObject*, bool);
    void destructClips();
    void setTimelineMovieClip(MovieClip*);

    bool touchPressed(Touch&);
};
#endif