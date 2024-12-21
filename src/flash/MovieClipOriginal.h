#ifndef MOVIECLIPORIGINAL_H
#define MOVIECLIPORIGINAL_H
#include "DisplayObjectOriginal.h"
#include "MovieClipFrame.h"
#include <vector>
#include "Rect.h"
#include "MovieClip.h"
class MovieClipOriginal :public DisplayObjectOriginal {
public:
    void load(SupercellSWF* sc);
    unsigned char fps;
    unsigned short frameSize;
    std::vector<MovieClipFrame> frames;
    unsigned short childSize;
    unsigned short* childrenIds;
    unsigned char* childrenBlendModes;
    char** childrenNames;
    Rect* scalingGrid;
    unsigned char matrixBankIndex;
    void createTimelineChildren(SupercellSWF* sc);
    DisplayObjectOriginal** timelineChildren;
    MovieClip* clone(SupercellSWF*, Rect*);
};
#endif