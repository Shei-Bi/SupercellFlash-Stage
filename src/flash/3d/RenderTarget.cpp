#include "RenderTarget.h"
#include <glad/glad.h>

RenderTarget::RenderTarget() {
    frameBuffer = 0;
    depthBuffer = 0;
    texture.id = 0;
    width = 0;
    height = 0;
}

void RenderTarget::init(int width, int height) {
    int previousTexture, previousFrameBuffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);

    this->width = width;
    this->height = height;

    if (frameBuffer == 0) {
        glGenFramebuffers(1, &frameBuffer);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    if (depthBuffer == 0) {
        glGenRenderbuffers(1, &depthBuffer);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    if (texture.id == 0) {
        glGenTextures(1, &texture.id);
    }
    texture.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.id, 0);

    glBindTexture(GL_TEXTURE_2D, previousTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
}

void RenderTarget::reshape(int width, int height) {
    if (this->width != width || this->height != height) {
        init(width, height);
    }
}

void RenderTarget::begin() {
    begin(0, 0, width, height);
}

void RenderTarget::begin(int x, int y, int width, int height) {
    glGetIntegerv(GL_VIEWPORT, (int*)&previousViewport);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (int*)&previousFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(x, y, width, height);
}

void RenderTarget::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    glViewport(previousViewport.left, previousViewport.top, previousViewport.right, previousViewport.bottom);
}
