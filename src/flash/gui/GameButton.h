#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H
#include "flash/Sprite.h"
#include "IButtonListener.h"

class MovieClip;
class GameButton :public Sprite {
    MovieClip* buttonClip;
    int state;
    bool touchIned;
public:
    MovieClip* timelineClip;
    const char* name;
    IButtonListener* buttonListener;
    GameButton();
    void setMovieClip(MovieClip*, bool);
    void setDisplayObject(DisplayObject*, bool);
    void destructClips();
    void setTimelineMovieClip(MovieClip*);
    void setState(int);
    void setEnabled(bool);
    bool isEnabled();
    int getButtonState();
    void updateTimelineClip(MovieClip*, int);
    const char* getStateStartFrameLabel(int);
    const char* getStateEndFrameLabel(int);
    void setButtonListener(IButtonListener*);

    bool touchPressed(Touch&);
    bool touchMoved(Touch&);
    bool touchReleased(Touch&);

    void buttonPressed();

    bool render(Matrix2x3*, ColorTransform* c, int, float);
};
#endif