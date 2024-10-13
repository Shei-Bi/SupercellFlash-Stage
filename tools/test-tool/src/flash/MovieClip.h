#pragma once
#include <flash/display_object/MovieClipOriginal.h>
#include <flash/Sprite.h>
#include <flash/types/SWFContainer.hpp>
#include <flash/display_object/MovieClipFrame.h>
#include <flash/transform/MatrixBank.h>

class MovieClip :public Sprite
{
private:
    DisplayObject** timelineChildren;
    int timelineChildrenCount;
    sc::flash::SWFVector<sc::flash::MovieClipFrame>* frames;
    sc::flash::MatrixBank* matrixBank;
    int frameIndex;
    float frameTime;
    float secondPerFrame;
public:
    static MovieClip* createMovieClip(sc::flash::MovieClipOriginal* movieClipOriginal, sc::flash::SupercellSWF* swf);
    MovieClip::MovieClip() :Sprite(-1) {
        ;
    }
    void setFrame(int);
    bool render(Matrix2x3*,ColorTransform* c, float);
};
