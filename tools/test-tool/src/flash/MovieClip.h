#pragma once
#include <flash/display_object/MovieClipOriginal.h>
#include <flash/Sprite.h>
#include <flash/types/SWFContainer.hpp>
#include <flash/display_object/MovieClipFrame.h>
#include <flash/transform/MatrixBank.h>
#include <flash/TextField.h>

class MovieClip :public Sprite
{
private:
    DisplayObject** timelineChildren;
    int timelineChildrenCount;
    sc::flash::SWFVector<sc::flash::MovieClipFrame>* frames;
    sc::flash::SWFVector<sc::flash::DisplayObjectInstance>* instances;
    sc::flash::MatrixBank* matrixBank;
    int currentFrame;
    float frameTime;
    float secondPerFrame;
    int state;
    int loopFrame;
    int totalFrames;
public:
    enum AnimateState {
        PLAYING,
        PLAYING_ANY_DIRECTION,
        STOPPED
    };
    static MovieClip* createMovieClip(sc::flash::MovieClipOriginal* movieClipOriginal, sc::flash::SupercellSWF* swf);
    MovieClip::MovieClip() :Sprite(-1) {
        ;
    }
    MovieClip::~MovieClip();
    void setFrame(int);
    bool render(Matrix2x3*, ColorTransform* c, int, float);
    MovieClip* getMovieClipByName(char*);
    TextField* getTextFieldByName(char*);
    void setChildVisible(char*, bool);
    int getTotalFrames();
    void gotoAndStopFrameIndex(int);
    void gotoAndPlayFrameIndex(int, int);
    void removeChildAt(short);
    void stop(void);

    void initScreenContainers(char*, std::vector<MovieClip*>&);
    MovieClip* createScreenContainer(char*, int);
    MovieClip* getMovieClipRecursive(char*);

    virtual bool isMovieClip() const;
};
