#include "MovieClipFrame.h"
#include "SupercellSWF.h"
unsigned short MovieClipFrame::load(SupercellSWF* sc) {
    unsigned short elementSize = sc->readShort();
    int stack;
    name = sc->readAscii(&stack, NULL);
    return elementSize;
}