#ifndef SHAPEDRAWBITMAPCOMMAND_H
#define SHAPEDRAWBITMAPCOMMAND_H
#include "ShapeDrawBitmapCommandVertex.h"
#include "GLImage.h"
#include "Matrix2x3.h"
#include "ColorTransform.h"
class SupercellSWF;
class ShapeDrawBitmapCommand {
public:
    unsigned char vertexSize;
    ShapeDrawBitmapCommandVertex* vertexs;
    GLImage* glImage;
    void load(SupercellSWF*, ShapeDrawBitmapCommandVertex*);
    unsigned short getVertexCount();
    bool render(Matrix2x3* mat, ColorTransform* c, int rc);
};
#endif