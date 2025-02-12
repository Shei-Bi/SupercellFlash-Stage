#pragma once
#include "RenderTarget.h"
#include "flash/MovieClip.h"

class FaceTexture {
public:
    int width, height;
    GLImage* texture;
    RenderTarget* renderTarget;
    MovieClip* movieClip;
    FaceTexture(int width, int height);
    void update(float deltaTime);
    void setMovieClip(MovieClip* movieClip);
};