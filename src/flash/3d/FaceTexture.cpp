#include "FaceTexture.h"
#include "flash/Stage.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

FaceTexture::FaceTexture(int width, int height) :width(width), height(height) {
    texture = new GLImage();
    renderTarget = nullptr;
    movieClip = nullptr;
}

void FaceTexture::update(float deltaTime) {
    if (!movieClip) return;
    if (!renderTarget) {
        renderTarget = new RenderTarget(1);
    }
    renderTarget->reshape(width, height);
    texture->createWrapper(renderTarget->texture.id);
    Stage* Stage = Stage::getInstance();
    Stage->renderBuckets();

    Stage->initShaders(glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f));
    renderTarget->begin();
    auto mat = new Matrix2x3();
    auto cT = new ColorTransform();
    movieClip->render(mat, cT, 0, deltaTime);
    Stage->renderBuckets();
    renderTarget->end();
    Stage->initShaders(Renderer::getPixelMatrix());
    //2 -2 0 2
    delete mat;
    delete cT;
}

void FaceTexture::setMovieClip(MovieClip* movieClip) {
    if (this->movieClip == movieClip) return;
    this->movieClip = movieClip;
}
