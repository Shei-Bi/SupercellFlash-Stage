#ifndef MOVIECLIPMODIFIER_H
#define MOVIECLIPMODIFIER_H
#include "DisplayObjectOriginal.h"
class MovieClipModifierOriginal :public DisplayObjectOriginal {
public:
    int type;
    void load(SupercellSWF* sc, int type);
    DisplayObject* clone(SupercellSWF*, Rect*);
};
#endif