#ifndef MOVIECLIPMODIFIER_H
#define MOVIECLIPMODIFIER_H
#include "DisplayObjectOriginal.h"
class MovieClipModifier :public DisplayObjectOriginal {
public:
    int type;
    void load(SupercellSWF* sc, int type);
};
#endif