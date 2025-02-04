#pragma once
#include "Material.h"

class ImpostorMaterial : public Material {
public:
    virtual void bind(const glm::mat4& transform) {
        diffuseTexture->bind();
        shader->use();
        shader->setVec2("u_outlineScale", glm::vec2(1.0f / 600.0f));
        shader->setVec4("u_outlineColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
};