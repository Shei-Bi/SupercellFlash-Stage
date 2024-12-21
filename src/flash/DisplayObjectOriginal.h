#ifndef DISPLAYOBJECTORIGINAL_H
#define DISPLAYOBJECTORIGINAL_H
class SupercellSWF;
class DisplayObject;
#include "Rect.h"
class DisplayObjectOriginal {
public:
    unsigned short id;
    virtual DisplayObject* clone(SupercellSWF*, Rect*);
};
#endif