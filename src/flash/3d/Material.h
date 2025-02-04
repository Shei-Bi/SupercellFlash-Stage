#pragma once
#include "Shader.h"
#include "GLImage.h"

class Material {
public:
    virtual void bind(const glm::mat4& transform) {
        diffuseTexture->bind();
        shader->use();
    }

    void setDiffuseTex(GLImage* image) {
        diffuseTexture = image;
    }

    GLImage* diffuseTexture;
    Shader* shader;
};