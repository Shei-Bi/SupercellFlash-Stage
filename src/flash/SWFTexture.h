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
    const unsigned char ktxHeader[12] = { 0xAB, 'K', 'T', 'X', ' ', '1', '1', 0xBB, '\r', '\n', 0x1A, '\n' };
    unsigned char** khronosDatas;
    int khronosDataSize;
    int levels;
    void load(SupercellSWF*);
    GLImage* glImage;
};
#endif