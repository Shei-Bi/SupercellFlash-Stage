#include "RenderTarget.h"
#include <glad/glad.h>

RenderTarget::RenderTarget(int samples) :samples(samples) {
    multisampledFrameBuffer = 0;
    frameBuffer = 0;
    depthBuffer = 0;
    multisampledTexture.id = 0;
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

    if (samples == 1) {
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.id, 0);
    }
    else {
        if (multisampledFrameBuffer == 0) {
            glGenFramebuffers(1, &multisampledFrameBuffer);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, multisampledFrameBuffer);

        if (depthBuffer == 0) {
            glGenRenderbuffers(1, &depthBuffer);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        if (multisampledTexture.id == 0) {
            glGenTextures(1, &multisampledTexture.id);
        }
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture.id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, true);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture.id, 0);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        //normal
        if (frameBuffer == 0) {
            glGenFramebuffers(1, &frameBuffer);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        if (texture.id == 0) {
            glGenTextures(1, &texture.id);
        }
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
    }
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

    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFrameBuffer != 0 ? multisampledFrameBuffer : frameBuffer);
    // glViewport(x, y, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::end() {
    if (multisampledFrameBuffer != 0) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
    // glViewport(previousViewport.left, previousViewport.top, previousViewport.right, previousViewport.bottom);
}
