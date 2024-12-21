#include "SupercellSWF.h"
#include "MovieClip.h"
#include <assert.h>
void MovieClipOriginal::load(SupercellSWF* sc) {
    id = sc->readShort();
    fps = sc->readUnsignedChar();
    frameSize = sc->readShort();
    frames.resize(frameSize + 1);
    int elementSize = sc->readInt() * 3;
    auto sharedElementArray = sc->readShortArray(elementSize);
    childSize = sc->readShort();
    childrenIds = sc->readShortArray(childSize);
    childrenBlendModes = sc->readByteArray(childSize);
    childrenNames = new char* [childSize];
    int stack;
    for (unsigned short i = 0;i < childSize;i++) {
        childrenNames[i] = sc->readAscii(&stack, NULL);
    }
    unsigned short loadedFrames = 0;
    int loadedElements = 0;
    while (true) {
        unsigned char tag = sc->readUnsignedChar();
        int l = sc->readInt();
        switch (tag) {
        case 0:
            // assert(loadedElements == elementSize);
            assert(loadedFrames == frameSize);
            frames[loadedFrames].elements = &sharedElementArray[loadedElements];
            return;
        case 11:
            frames[loadedFrames].elements = &sharedElementArray[loadedElements];
            loadedElements += frames[loadedFrames].load(sc) * 3;
            loadedFrames++;
            break;
        case 31:
            scalingGrid = Rect::LTWH(sc->readTwip(), sc->readTwip(), sc->readTwip(), sc->readTwip());
            break;
        case 41:
            matrixBankIndex = sc->readUnsignedChar();
            break;
        }
    }
}
void MovieClipOriginal::createTimelineChildren(SupercellSWF* sc) {
    if (timelineChildren == NULL) {
        timelineChildren = new DisplayObjectOriginal * [childSize];
        for (unsigned short i = 0;i < childSize;i++) {
            timelineChildren[i] = sc->getOriginalDisplayObject(childrenIds[i]);
        }
    }
}
MovieClip* MovieClipOriginal::clone(SupercellSWF* sc, Rect* rect) {
    return MovieClip::createMovieClip(this, sc);
}