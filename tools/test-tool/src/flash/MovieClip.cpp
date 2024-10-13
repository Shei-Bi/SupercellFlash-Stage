#include <flash/MovieClip.h>
#include <flash/Sprite.h>
#include <flash/display_object/DisplayObjectOriginal.h>
#include <flash/Shape.h>
#include <stdio.h>
#include <flash/objects/SupercellSWF.h>

MovieClip* MovieClip::createMovieClip(sc::flash::MovieClipOriginal* movieClipOriginal, sc::flash::SupercellSWF* swf) {
    movieClipOriginal->createTimelineChildren(swf);
    MovieClip* movieClip = new MovieClip();
    movieClip->timelineChildrenCount = movieClipOriginal->instances.size();
    movieClip->allocateMemory(movieClip->timelineChildrenCount);

    movieClip->timelineChildren = new DisplayObject * [movieClip->timelineChildrenCount];
    for (int i = 0;i < movieClip->timelineChildrenCount;i++) {
        sc::flash::DisplayObjectOriginal* a = movieClipOriginal->displayObjects[i];
        DisplayObject* b = nullptr;
        if (a->is_movieclip()) b = MovieClip::createMovieClip((sc::flash::MovieClipOriginal*)a, swf);
        if (a->is_shape()) {
            b = Shape::createShape((sc::flash::ShapeOriginal*)a);
            for (sc::flash::ShapeDrawBitmapCommand c : *((Shape*)b)->commands) printf("texture_index:%d\n", c.texture_index);
        }
        // if (a->is()) b = nullptr;
        // if (a->is_movieclip()) b = nullptr;
        movieClip->timelineChildren[i] = b;
        //b.setInteractiveResureiosx']/
    }
    // printf("%d", movieClipOriginal->frames[0].frameElements[0]->colorTransform_index);
    movieClip->matrixBank = &swf->matrixBanks[movieClipOriginal->bank_index];
    movieClip->frames = &movieClipOriginal->frames;
    movieClip->setFrame(0);
    movieClip->frameTime = 0.0f;
    movieClip->secondPerFrame = 1.0f / movieClipOriginal->frame_rate;
    return movieClip;
}
void MovieClip::setFrame(int index) {
    frameIndex = index;
    int childIndex = 0;
    sc::flash::MovieClipFrame frame = (*frames)[index];
    for (int i = 0;i < frame.elements_count;i++) {
        sc::flash::MovieClipFrameElement* element = frame.frameElements[i];
        DisplayObject* child = timelineChildren[element->instance_index];
        if (child == nullptr) continue;
        if (element->matrix_index != 65535) child->Matrix = matrixBank->matrices[element->matrix_index];
        if (element->colorTransform_index != 65535) child->colorTransform = matrixBank->color_transforms[element->colorTransform_index];
        addChildAt(child, childIndex++);
    }
    for (int i = size - 1;i >= childIndex;i--) removeChildAt(i);
}
bool MovieClip::render(Matrix2x3* mat, ColorTransform* c, float deltaTime) {
    if (frameTime >= secondPerFrame) {
        int framePassed = (int)(frameTime / secondPerFrame);
        frameTime -= framePassed * secondPerFrame;
        setFrame((frameIndex + framePassed) % (*frames).size());
    }
    frameTime += deltaTime;
    return Sprite::render(mat, c, deltaTime);
}