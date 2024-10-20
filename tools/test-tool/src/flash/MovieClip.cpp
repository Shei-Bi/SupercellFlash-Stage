#include <flash/MovieClip.h>
#include <flash/Sprite.h>
#include <flash/display_object/DisplayObjectOriginal.h>
#include <flash/Shape.h>
#include <stdio.h>
#include <flash/objects/SupercellSWF.h>
#include <flash/Shape9Slice.h>
#include <flash/TextField.h>

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
    for (int i = size - 1;i >= childIndex;i--) removeChildAt(i);
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