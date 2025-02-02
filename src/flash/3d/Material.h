#pragma once
#include "Shader.h"
#include "GLImage.h"

class Material {
public:
    void bind() {
        diffuseTexture->bind();
        shader->use();
    }

    void setDiffuseTex(GLImage* image) {
        diffuseTexture = image;
    }

    GLImage* diffuseTexture;
    Shader* shader;
};