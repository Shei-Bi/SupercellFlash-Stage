#ifndef SHAPEORIGINAL_H
#define SHAPEORIGINAL_H
#include <vector>
#include "DisplayObjectOriginal.h"
#include "ShapeDrawBitmapCommand.h"
#include "Shape.h"
class ShapeOriginal :public DisplayObjectOriginal {
public:
    std::vector<ShapeDrawBitmapCommandVertex> sharedVertexArray;
    std::vector<ShapeDrawBitmapCommand> commands;
    unsigned short commandSize;
    void load(SupercellSWF*);
    Shape* clone(SupercellSWF*, Rect*);
};
#endif