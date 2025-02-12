#pragma once
#include "Material.h"

class CharacterMaterial : public Material {
public:
    virtual void bind(const glm::mat4& transform) {
        Material::bind(transform);
        shader->setMat4("view_matrix", transform);
        shader->setFloat("a_texMul", 1.0f);
    }
};