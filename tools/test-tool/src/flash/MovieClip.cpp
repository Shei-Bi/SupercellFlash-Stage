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
        if (a->is_shape()) b = Shape::createShape((sc::flash::ShapeOriginal*)a);
        // if (a->is()) b = nullptr;
        // if (a->is_movieclip()) b = nullptr;
        movieClip->timelineChildren[i] = b;
        //b.setInteractiveResureiosx']/
    }
    // printf("%d", movieClipOriginal->frames[0].frameElements[0]->colorTransform_index);
    movieClip->matrixBank = &swf->matrixBanks[movieClipOriginal->bank_index];
    movieClip->frames = &movieClipOriginal->frames;
    movieClip->setFrame(0);
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
        addChildAt(child, childIndex++);
    }
    for (int i = size - 1;i >= childIndex;i--) removeChildAt(i);
}
bool MovieClip::render(Matrix2x3* mat) {
    setFrame((frameIndex + 1) % (*frames).size());
    return Sprite::render(mat);
}