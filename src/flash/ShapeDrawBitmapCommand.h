#ifndef SHAPEDRAWBITMAPCOMMAND_H
#define SHAPEDRAWBITMAPCOMMAND_H
#include "ShapeDrawBitmapCommandVertex.h"
#include "GLImage.h"
class SupercellSWF;
class ShapeDrawBitmapCommand {
public:
    unsigned char vertexSize;
    ShapeDrawBitmapCommandVertex* vertexs;
    GLImage* glImage;
    void load(SupercellSWF*, ShapeDrawBitmapCommandVertex*);
    unsigned short getVertexCount();
};
#endif