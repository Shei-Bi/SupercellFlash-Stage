#ifndef SWFTEXTURE_H
#define SWFTEXTURE_H
#include "DisplayObjectOriginal.h"
#include "GLImage.h"
class SWFTexture {
public:
    int glFormat;
    int glInternalFormat;
    unsigned char pixelFormat;
    unsigned short width;
    unsigned short height;
    static const unsigned char ktxHeader[12];
    unsigned char** khronosDatas;
    int khronosDataSize;
    int levels;
    void load(SupercellSWF*);
    GLImage* glImage;
};
#endif