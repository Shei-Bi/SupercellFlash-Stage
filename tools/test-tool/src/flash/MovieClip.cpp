#include <flash/MovieClip.h>
#include <flash/Sprite.h>
#include <flash/display_object/DisplayObjectOriginal.h>
#include <flash/Shape.h>
#include <stdio.h>
#include <flash/objects/SupercellSWF.h>
#include <flash/Shape9Slice.h>
#include <flash/TextField.h>
#include <ResourceManager.h>
#include <flash/Stage.h>

int BLEND_MODE_MAP[] = { 0, 0, 0, 0x100, 0x180, 0, 0, 0, 0x80, 0, 0, 0, 0x200, 0, 0, 0x200 };
MovieClip* MovieClip::createMovieClip(sc::flash::MovieClipOriginal* movieClipOriginal, sc::flash::SupercellSWF* swf) {
    movieClipOriginal->createTimelineChildren(swf);
    MovieClip* movieClip = new MovieClip();
    movieClip->currentFrame = -1;
    movieClip->loopFrame = -1;
    movieClip->state = PLAYING;
    movieClip->timelineChildrenCount = movieClipOriginal->instances.size();
    movieClip->allocateMemory(movieClip->timelineChildrenCount);

    movieClip->timelineChildren = new DisplayObject * [movieClip->timelineChildrenCount];
    for (int i = 0;i < movieClip->timelineChildrenCount;i++) {
        sc::flash::DisplayObjectOriginal* a = movieClipOriginal->displayObjects[i];
        DisplayObject* b = nullptr;
        if (a->is_movieclip()) b = MovieClip::createMovieClip((sc::flash::MovieClipOriginal*)a, swf);
        if (a->is_shape()) {
            b = movieClipOriginal->scaling_grid.has_value() ? Shape9Slice::createShape((sc::flash::ShapeOriginal*)a, &movieClipOriginal->scaling_grid.value()) : Shape::createShape((sc::flash::ShapeOriginal*)a);
        }
        // if (a->is()) b = nullptr;
        // if (a->is_movieclip()) b = nullptr;
        if (b) b->setBlendMode(BLEND_MODE_MAP[(int)movieClipOriginal->instances[i].blend_mode & 0x3F]);
        movieClip->timelineChildren[i] = b;
        //b.setInteractiveResureiosx']/
    }
    // printf("%d", movieClipOriginal->frames[0].frameElements[0]->colorTransform_index);
    movieClip->matrixBank = &swf->matrixBanks[movieClipOriginal->bank_index];
    movieClip->frames = &movieClipOriginal->frames;
    movieClip->instances = &movieClipOriginal->instances;
    movieClip->setFrame(0);
    movieClip->frameTime = 0.0f;
    movieClip->secondPerFrame = 1.0f / movieClipOriginal->frame_rate;
    movieClip->totalFrames = movieClipOriginal->frames.size();
    return movieClip;
}
void MovieClip::setFrame(int index) {
    if (loopFrame == index) state = STOPPED;
    if (currentFrame == index) return;
    currentFrame = index;
    int childIndex = 0;
    sc::flash::MovieClipFrame* frame = &(*frames)[index];
    for (int i = 0;i < frame->elements_count;i++) {
        sc::flash::MovieClipFrameElement* element = frame->frameElements[i];
        DisplayObject* child = timelineChildren[element->instance_index];
        if (child == nullptr) continue;
        if (element->matrix_index != 65535) child->Matrix = matrixBank->matrices[element->matrix_index];
        if (element->colorTransform_index != 65535) child->colorTransform = matrixBank->color_transforms[element->colorTransform_index];
        addChildAt(child, childIndex++);
    }
    for (int i = size - 1;i >= childIndex;i--) Sprite::removeChildAt(i);
}
bool MovieClip::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    if (deltaTime <= 0.0) goto skip;
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
MovieClip* MovieClip::getMovieClipByName(char* name) {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if ((*instances)[i].name == name) return (MovieClip*)timelineChildren[i];
    }
    return nullptr;
}
TextField* MovieClip::getTextFieldByName(char* name) {
    for (int i = 0;i < timelineChildrenCount;i++) {
        if ((*instances)[i].name == name) return (TextField*)timelineChildren[i];
    }
    return nullptr;
}
void MovieClip::setChildVisible(char* name, bool v) {
    getMovieClipByName(name)->visible = v;
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
MovieClip* MovieClip::createScreenContainer(char* name, int index) {
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
        y *= 0.5f;
        break;
    case 6:
        s = "hud_right";
        y *= 0.5f;
        break;
    case 7:
        s = "hud_top_left";
        y = 0.0f;
        break;
    case 8:
        s = "hud_top_right";
        y *= 0.5f;
        break;
    case 9:
        s = "hud_bottom_left";
        break;
    }
    sc::flash::SupercellSWF* supercellSWF = ResourceManager::getSupercellSWF("sc/ui.sc", nullptr);

    MovieClip* c = nullptr;
    if (supercellSWF->hasExportName((char*)(std::string(name) + s).c_str())) {
        c = ResourceManager::getMovieClip("sc/ui.sc", (char*)(std::string(name) + s).c_str());
        addChild(c);
        c->setPixelSnappedXY(x, y);
    }
    return c;
}
void MovieClip::initScreenContainers(char* name, std::vector<MovieClip*>& vector) {
    for (int i = 0;i < 10;i++) {
        vector.push_back(createScreenContainer(name, i));
    }
}
MovieClip* MovieClip::getMovieClipRecursive(char* name) {
    DisplayObject* e = nullptr;
    for (int i = 0;i < timelineChildrenCount;i++) {
        e = timelineChildren[i];
        if (e) {
            if ((*instances)[i].name == name) break;
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