#pragma once
#include "GLImage.h"
#include "flash/Rect.h"

class RenderTarget {
public:
    unsigned int frameBuffer;
    unsigned int multisampledFrameBuffer;
    unsigned int depthBuffer;
    int width, height;
    int samples;
    GLImage multisampledTexture;
    GLImage texture;
    struct
    {
        int x, y, width, height;
    } previousViewport;
    unsigned int previousFrameBuffer;
    RenderTarget(int samples);
    void init(int width, int height);
    void reshape(int width, int height);
    void begin();
    void begin(int x, int y, int width, int height);
    void end();
};