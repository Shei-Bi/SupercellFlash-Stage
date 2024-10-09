#pragma once
#include <flash/display_object/MovieClipOriginal.h>
#include <flash/Sprite.h>
// #include <
class MovieClip :public Sprite
{
private:
    /* data */
public:
    static MovieClip* createMovieClip(sc::flash::MovieClipOriginal movieClipOriginal, sc::flash::SupercellSWF& swf) {
        movieClipOriginal.createTimelineChildren(swf);
    }
};
