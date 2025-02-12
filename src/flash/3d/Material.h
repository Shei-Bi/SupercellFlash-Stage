#pragma once
#include "Shader.h"
#include "GLImage.h"

class Material {
public:
    Material() {
        diffuseTexture = nullptr;
        stencilTexture = nullptr;
        shader = nullptr;
    }

    virtual void bind(const glm::mat4& transform) {
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        diffuseTexture->bind();
        if (stencilTexture) {
            glActiveTexture(GL_TEXTURE1);
            stencilTexture->bind();
            shader->setInt("stencilTex", 1);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    void setDiffuseTex(GLImage* image) {
        diffuseTexture = image;
    }

    void setStencilTex(GLImage* image) {
        stencilTexture = image;
    }

    GLImage* diffuseTexture;
    GLImage* stencilTexture;
    Shader* shader;
};