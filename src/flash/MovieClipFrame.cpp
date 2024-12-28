#include "MovieClipFrame.h"
#include "SupercellSWF.h"
unsigned short MovieClipFrame::load(SupercellSWF* sc) {
    unsigned short elementSize = sc->readShort();
    int stack;
    name = sc->readAscii(&stack, NULL);
    return elementSize;
}
bool MovieClipFrame::containsTimelineChild(short childIndex, int length) {
    for (int i = 0;i < length;i += 3) {
        if (elements[i] == childIndex) return true;
    }
    return false;
}