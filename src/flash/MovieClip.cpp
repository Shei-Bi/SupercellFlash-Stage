#include <flash/MovieClip.h>
#include <flash/Sprite.h>
// #include <flash/display_object/DisplayObjectOriginal.h>
#include <flash/Shape.h>
#include <stdio.h>
#include <flash/SupercellSWF.h>
#include <flash/Shape9Slice.h>
#include <flash/TextField.h>
#include <ResourceManager.h>
#include <flash/Stage.h>
#include "flash/gui/GameButton.h"

#pragma optimize("",off);

int BLEND_MODE_MAP[] = { 0, 0, 0, 0x100, 0x180, 0, 0, 0, 0x80, 0, 0, 0, 0x200, 0, 0, 0x200 };
MovieClip* MovieClip::createMovieClip(MovieClipOriginal* movieClipOriginal, SupercellSWF* swf) {
    movieClipOriginal->createTimelineChildren(swf);
    MovieClip* movieClip = new MovieClip();
    movieClip->currentFrame = -1;
    movieClip->loopFrame = -1;
    movieClip->state = PLAYING;
    movieClip->timelineChildrenCount = movieClipOriginal->childSize;
    movieClip->allocateMemory(movieClip->timelineChildrenCount);

    movieClip->timelineChildren = new DisplayObject * [movieClip->timelineChildrenCount];
    for (int i = 0;i < movieClip->timelineChildrenCount;i++) {
        DisplayObjectOriginal* a = movieClipOriginal->timelineChildren[i];
        DisplayObject* b = a->clone(swf, movieClipOriginal->scalingGrid);
        // if (a) b = MovieClip::createMovieClip((sc::flash::MovieClipOriginal*)a, swf);
        // if (a->is_shape()) {
        //     b = movieClipOriginal->scaling_grid.has_value() ? Shape9Slice::createShape((sc::flash::ShapeOriginal*)a, &movieClipOriginal->scaling_grid.value()) : Shape::createShape((sc::flash::ShapeOriginal*)a);
        // }
        // if (a->is()) b = nullptr;
        // if (a->is_movieclip()) b = nullptr;
        if (b) b->setBlendMode(BLEND_MODE_MAP[(int)movieClipOriginal->childrenBlendModes[i] & 0x3F]);
        movieClip->timelineChildren[i] = b;
        //b.setInteractiveResureiosx']/
    }
    // printf("%d", movieClipOriginal->frames[0].frameElements[0]->colorTransform_index);
    movieClip->matrixBank = swf->matrixBanks[movieClipOriginal->matrixBankIndex];
    movieClip->frames = movieClipOriginal->frames.data();
    movieClip->frameSize = movieClipOriginal->frameSize;
    // movieClip->instances = &movieClipOriginal->instances;
    movieClip->setFrame(0);
    movieClip->frameTime = 0.0f;
    movieClip->secondPerFrame = 1.0f / movieClipOriginal->fps;
    movieClip->totalFrames = movieClipOriginal->frameSize;
    movieClip->childrenNames = movieClipOriginal->childrenNames;
    movieClip->childrenIds = movieClipOriginal->childrenIds;
    return movieClip;
}
void MovieClip::setFrame(int index) {
    if (loopFrame == index) state = STOPPED;
    if (currentFrame == index) return;
    currentFrame = index;
    int childIndex = 0;
    if (frameSize < 1) return;
    for (unsigned short* element = frames[index].elements;element < frames[index + 1].elements;element += 3) {
        // printf("%d\n", frames[index + 1].elements - element);
        DisplayObject* child = timelineChildren[element[0]];
        if (child == nullptr) continue;
        if (element[1] != 65535) child->Matrix = matrixBank->matrixs[element[1]];
        else child->Matrix.reset();
        if (element[2] != 65535) child->colorTransform = matrixBank->colorTransforms[element[2]];
        else child->colorTransform.reset();
        addChildAt(child, childIndex++);
    }
    for (int i = size - 1;i >= childIndex;i--) Sprite::removeChildAt(i);
}
bool MovieClip::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    if (deltaTime <= 0.0 || totalFrames == 0) goto skip;
    if (frameTime >= secondPerFrame) {
        int framePassed = (int)(frameTime / secondPerFrame);
        frameTime -= framePassed * secondPerFrame;
        int nextFrame;
        if (state == PLAYING_ANY_DIRECTION) {
            if (loopFrame >= currentFrame) {
                nextFrame = currentFrame + framePassed;
                if (nextFrame > loopFrame) {
                    nextFrame = loopFrame;
                }
            }
            else {
                nextFrame = currentFrame - framePassed;
                if (nextFrame < loopFrame) {
                    nextFrame = loopFrame;
                }
            }
        }
        else {
            nextFrame = currentFrame + framePassed;
            if (currentFrame < loopFrame && nextFrame > loopFrame) {
                nextFrame = loopFrame;
            }
        }
        setFrame(nextFrame % totalFrames);
    }
    if (state != STOPPED) frameTime += deltaTime;
skip:
    return Sprite::render(mat, c, rc, deltaTime);
}
MovieClip* MovieClip::getMovieClipByName(const char* name) {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if (childrenNames[i] && strcmp(childrenNames[i], name) == 0) return (MovieClip*)timelineChildren[i];
    }
    return nullptr;
}
TextField* MovieClip::getTextFieldByName(const char* name) {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if (childrenNames[i] && strcmp(childrenNames[i], name) == 0) return (TextField*)timelineChildren[i];
    }
    return nullptr;
}
void MovieClip::setChildVisible(const char* name, bool v) {
    auto mc = getMovieClipByName(name);
    if (mc) mc->visible = v;
}
int MovieClip::getTotalFrames() {
    return totalFrames;
}
void MovieClip::gotoAndStopFrameIndex(int index) {
    gotoAndPlayFrameIndex(index, -1);
    if (state != STOPPED) {
        state = STOPPED;
        frameTime = 0.0f;
    }
}
void MovieClip::gotoAndPlayFrameIndex(int index, int loopFrame) {
    this->loopFrame = loopFrame;
    if (index >= 0 && index < totalFrames) setFrame(index);
    if (index == loopFrame) {
        if (state != STOPPED) {
            state = STOPPED;
            frameTime = 0.0f;
        }
    }
    else {
        if (state != PLAYING) {
            state = PLAYING;
            frameTime = 0.0f;
        }
    }
}
void MovieClip::stop() {
    if (state != STOPPED) {
        state = STOPPED;
        frameTime = 0.0f;
    }
}
void MovieClip::removeChildAt(short index) {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if (timelineChildren[i] == children[index]) timelineChildren[i] = nullptr;
    }
    Sprite::removeChildAt(index);
}
MovieClip* MovieClip::createScreenContainer(const char* name, int index) {
    std::string s("");
    float x = Stage::getInstance()->matrixX;
    float y = Stage::getInstance()->matrixY;
    switch (index) {
    case 0:
        s = "bg";
        x *= 0.5f;
        y *= 0.5f;
        break;
    case 1:
        s = "center";
        x *= 0.5f;
        y *= 0.5f;
        break;
    case 2:
        s = "hud_top";
        x *= 0.5f;
        y = 0.0f;
        break;
    case 3:
        s = "hud_bottom";
        x *= 0.5f;
        break;
    case 4:
        s = "hud_bottom_right";
        break;
    case 5:
        s = "hud_left";
        x = 0.0f;
        y *= 0.5f;
        break;
    case 6:
        s = "hud_right";
        y *= 0.5f;
        break;
    case 7:
        s = "hud_top_left";
        x = 0.0f;
        y = 0.0f;
        break;
    case 8:
        s = "hud_top_right";
        y = 0.0f;
        break;
    case 9:
        s = "hud_bottom_left";
        x = 0.0f;
        break;
    }
    SupercellSWF* supercellSWF = ResourceManager::getSupercellSWF("sc/ui.sc", nullptr);

    MovieClip* c = nullptr;
    if (supercellSWF->hasExportName((char*)(std::string(name) + s).c_str())) {
        c = ResourceManager::getMovieClip("sc/ui.sc", (char*)(std::string(name) + s).c_str());
        addChild(c);
        c->setPixelSnappedXY(x, y);
    }
    return c;
}
void MovieClip::initScreenContainers(const char* name, std::vector<MovieClip*>& vector) {
    for (int i = 0;i < 10;i++) {
        vector.push_back(createScreenContainer(name, i));
    }
}
MovieClip* MovieClip::getMovieClipRecursive(const char* name) {
    DisplayObject* e = nullptr;
    for (int i = 0;i < timelineChildrenCount;i++) {
        e = timelineChildren[i];
        if (e) {
            if (childrenNames[i] && strcmp(childrenNames[i], name) == 0) break;
            if (e->isMovieClip()) {
                e = ((MovieClip*)e)->getMovieClipRecursive(name);
                if (e) break;
            }
        }
    }
    if (e && e->isMovieClip()) return (MovieClip*)e;
    return nullptr;
}
bool MovieClip::isMovieClip() const {
    return true;
}
MovieClip::~MovieClip() {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if (timelineChildren[i]) delete timelineChildren[i];
    }
    if (timelineChildren) delete[] timelineChildren;
    timelineChildren = nullptr;
}
MovieClip::MovieClip() :Sprite(-1) {
    ;
}
void MovieClip::gotoAndStop(const char* frameName) {
    gotoAndStopFrameIndex(getFrameIndex(frameName));
}
int MovieClip::getFrameIndex(const char* name) {
    if (!name) return -1;
    if (frameSize < 1) return -1;
    for (unsigned short i = 0;i < frameSize - 1;i++) {
        if (strcmp(frames[i].name, name) == 0) return i;
    }
    abort();//should be
}
void MovieClip::autoCreateButtons(std::vector<GameButton*>& out) {
    for (unsigned short i = 0;i < timelineChildrenCount;i++) {
        DisplayObject* d = timelineChildren[i];
        if (!d || !d->isMovieClip()) continue;
        MovieClip* movieClip = (MovieClip*)d;
        std::string name("");
        if (childrenNames[i]) name = childrenNames[i];
        // printf("%zd\n", name.find("button"));
        if (name.find("button") == std::string::npos) {
            movieClip->autoCreateButtons(out);
            continue;
        }
        // printf("%s\n", name.c_str());
        int index = getChildIndex(movieClip);
        GameButton* gameButton = new GameButton();
        gameButton->name = childrenNames[i];
        out.push_back(gameButton);
        changeTimelineChild(movieClip, gameButton);
        movieClip->Matrix.reset();
        gameButton->setMovieClip(movieClip, true);
    }
}
void MovieClip::changeTimelineChild(DisplayObject* from, DisplayObject* to) {
    for (unsigned short i = 0;i < timelineChildrenCount;i++) {
        if (timelineChildren[i] != from) continue;
        int index = from->indexInParent;
        if (index == -1) {
            if (to->parent) to->parent->removeChild(to);
        }
        else {
            to->Matrix = from->Matrix;
            to->colorTransform = from->colorTransform;
            removeChildAt(index);
            addChildAt(to, index);
            timelineChildren[i] = to;
        }
        break;
    }
}
void MovieClip::changeTimelineChild(const char* fromName, DisplayObject* to) {
    for (unsigned short i = 0;i < timelineChildrenCount;i++) {
        if (strcmp(childrenNames[i], fromName) == 0) {
            auto tlc = timelineChildren[i];
            if (tlc) {
                if (tlc != to) {
                    changeTimelineChild(tlc, to);
                    delete tlc;
                }
            }
            else {
                timelineChildren[i] = to;
                if (frames[currentFrame].containsTimelineChild(i, frames[currentFrame + 1].elements - frames[currentFrame].elements)) {
                    int t = currentFrame;
                    currentFrame = -1;
                    setFrame(t);
                }
            }
            return;
        }
    }
}
void MovieClip::moveThisToTopLayer() {
    if (parent) parent->addChildAt(this, parent->size);
}