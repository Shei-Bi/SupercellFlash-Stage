#pragma once
#include "GLImage.h"
#include "Rect.h"

class RenderTarget {
public:
    unsigned int frameBuffer;
    unsigned int depthBuffer;
    int width, height;
    GLImage texture;
    Rect previousViewport;
    unsigned int previousFrameBuffer;
    RenderTarget();
    void init(int width, int height);
    void reshape(int width, int height);
    void begin();
    void begin(int x, int y, int width, int height);
    void end();
};