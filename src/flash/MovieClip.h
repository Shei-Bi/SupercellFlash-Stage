#pragma once
class MovieClipOriginal;
#include <flash/Sprite.h>
#include "MovieClipFrame.h"
#include "ScMatrixBank.hpp"
#include <flash/TextField.h>
#include <vector>
#include "flash/gui/GameButton.h"
class MovieClip :public Sprite
{
private:
    DisplayObject** timelineChildren;
    int timelineChildrenCount;
    MovieClipFrame* frames;
    // std::vector<MovieClipFrame>* frames;
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
    MovieClip();
    ~MovieClip();
    void setFrame(int);
    bool render(Matrix2x3*, ColorTransform* c, int, float);
    void setInteractiveRecursive(bool);
    DisplayObject* getChildByName(const char*);
    MovieClip* getMovieClipByName(const char*);
    TextField* getTextFieldByName(const char*);
    void setChildVisible(const char*, bool);
    int getTotalFrames();
    void gotoAndStopFrameIndex(int);
    void gotoAndPlayFrameIndex(int, int);
    void playOnce();
    void removeChildAt(short);
    void stop(void);
    int getFrameIndex(const char*);
    void gotoAndStop(const char*);
    void changeTimelineChild(DisplayObject*, DisplayObject*);
    void changeTimelineChild(const char*, DisplayObject*);

    void initScreenContainers(const char*, std::vector<MovieClip*>&);
    MovieClip* createScreenContainer(const char*, int);
    MovieClip* getMovieClipRecursive(const char*);

    //MovieClipHelper
    void autoCreateButtons(std::vector<GameButton*>&);

    //Unknown
    void moveThisToTopLayer();

    void debugPrintChildNames();
    bool debugForceNewBucket;

    virtual bool isMovieClip() const;
};
