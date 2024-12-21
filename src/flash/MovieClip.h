#pragma once
class MovieClipOriginal;
#include <flash/Sprite.h>
#include "MovieClipFrame.h"
#include "ScMatrixBank.hpp"
#include <flash/TextField.h>
#include <vector>
class MovieClip :public Sprite
{
private:
    DisplayObject** timelineChildren;
    int timelineChildrenCount;
    // MovieClipFrame* frames;
    std::vector<MovieClipFrame>* frames;
    unsigned short frameSize;
    unsigned short* childrenIds;
    unsigned char* childrenBlendModes;
    char** childrenNames;
    ScMatrixBank* matrixBank;
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
    static MovieClip* createMovieClip(MovieClipOriginal* movieClipOriginal, SupercellSWF* swf);
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
