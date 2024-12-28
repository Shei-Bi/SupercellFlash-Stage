#ifndef MOVIECLIPFRAME_H
#define MOVIECLIPFRAME_H
#include "DisplayObjectOriginal.h"
class MovieClipFrame {
public:
    unsigned short* elements;
    // unsigned short elementSize;
    char* name;
    unsigned short load(SupercellSWF* sc);
    bool containsTimelineChild(short childIndex, int length);
};
#endif