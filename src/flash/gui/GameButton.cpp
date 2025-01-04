#include "GameButton.h"
#include "stdlib.h"
#include "flash/MovieClip.h"
#include "assert.h"
#include "ResourceManager.h"

GameButton::GameButton() :Sprite(1) {
    name = "No name";
    timelineClip = nullptr;
    buttonClip = nullptr;
    interactable = true;
    state = 0;
    buttonListener = nullptr;
    touchIned = false;
}
void GameButton::setMovieClip(MovieClip* movieClip, bool timeline) {
    setDisplayObject(movieClip, timeline);
}
void GameButton::setDisplayObject(DisplayObject* movieClip, bool timeline) {
    timelineClip = (MovieClip*)movieClip;
    if (timeline) {
        setTimelineMovieClip(ResourceManager::getMovieClip("sc/ui.sc", "button_timeline"));
    }
    else {
        addChild(timelineClip);
        buttonClip = (MovieClip*)movieClip;
    }
    int tempState = state;
    state = -1;
    setState(0);
    // updateHitArea();
    setState(tempState);
}
void GameButton::destructClips() {
    if (timelineClip != buttonClip) {
        if (timelineClip) delete timelineClip;
        timelineClip = nullptr;
    }
    if (buttonClip) delete buttonClip;
    buttonClip = nullptr;
}
void GameButton::setTimelineMovieClip(MovieClip* movieClip) {
    movieClip->interactable = true;
    movieClip->Matrix = timelineClip->Matrix;
    auto throwAway = movieClip->children[0];
    movieClip->changeTimelineChild(throwAway, timelineClip);
    if (throwAway) delete throwAway;
    addChild(movieClip);
    buttonClip = movieClip;
    int tempState = state;
    state = -1;
    setState(tempState);
}
bool GameButton::touchPressed(Touch& touch) {
    if (state != -1000) {
        setState(1);
    }
    Sprite::touchPressed(touch);
    touchIned = true;
    return true;
}
void GameButton::setState(int mstate) {
    if (state != mstate && state != -1000) {
        state = mstate;
        updateTimelineClip(buttonClip, mstate);
    }
}
void GameButton::updateTimelineClip(MovieClip* clip, int state) {
    if (!clip) return;
    // printf("%d\n", state);
    int start = clip->getFrameIndex(getStateStartFrameLabel(state));
    int end = clip->getFrameIndex(getStateEndFrameLabel(state));
    if (start != -1) {
        if (end != -1)
            clip->gotoAndPlayFrameIndex(start, end);
        else
            clip->gotoAndStopFrameIndex(start);
    }
}

const char* GameButton::getStateStartFrameLabel(int state) {
    switch (state) {
    case 0:return "Idle";
    case 1:return "Down";
    case 2:return "Clicked";
    case -1000: return "Disabled";
    default: assert(false);
    }
}

const char* GameButton::getStateEndFrameLabel(int state) {
    switch (state) {
    case 0:return "IdleEnd";
    case 1:return "DownEnd";
    case 2:return "ClickedEnd";
    case -1000: return "DisabledEnd";
    default: assert(false);
    }
}

void GameButton::setButtonListener(IButtonListener* i)
{
    buttonListener = i;
}

bool GameButton::isEnabled() {
    return state != -1000;
}

void GameButton::setEnabled(bool e) {
    if (e) {
        if (state == -1000) {
            state = -1;
            setState(0);
        }
    }
    else setState(-1000);
}

int GameButton::getButtonState() {
    return state;
}

bool GameButton::touchMoved(Touch&) {
    return true;
}

bool GameButton::touchReleased(Touch&) {
    if (state != -1000 && touchIned) {
        setState(2);
        buttonPressed();
    }
    // Sprite::touchPressed(touch);
    return true;
}

void GameButton::buttonPressed() {
    if (buttonListener) buttonListener->buttonClicked(this);
    printf("GameButton::buttonPressed():\t%s\n", name);
}

bool GameButton::render(Matrix2x3* mat, ColorTransform* c, int rc, float dT) {
    bool result = Sprite::render(mat, c, rc, dT);
    if (state == 2) {
        if (buttonClip && buttonClip->isStopped()) setState(0);
    }
    return result;
}